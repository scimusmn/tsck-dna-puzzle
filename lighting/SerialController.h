#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H

#include <Arduino.h>
#include <string.h>

#ifndef MAX_KEY_LEN
#define MAX_KEY_LEN 32
#endif

#ifndef MAX_VAL_LEN
#define MAX_VAL_LEN 32
#endif

#define MAX_MSG_LEN (MAX_KEY_LEN + MAX_VAL_LEN + 3)

typedef void (*voidCallback)();
typedef void (*stringCallback)(char*);
typedef void (*intCallback)(int);
typedef void (*floatCallback)(float);

void strappend(char *str, char c, int maxLength)
{
  snprintf(str, maxLength, "%s%c", str, c);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * SerialCallback
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

struct SerialCallback {
  char key[MAX_KEY_LEN];

  typedef enum {
    VOID,
    STRING,
    INT,
    FLOAT
  } ValueType;

  ValueType valueType;

  union {
    voidCallback v;
    stringCallback s;
    intCallback i;
    floatCallback f;
  } callback;

  void respond(char *value) {
    switch (valueType) {
    case VOID:
      callback.v();
      break;
      
    case STRING:
      callback.s(value);
      break;

    case INT:
      callback.i(atoi(value));
      break;

    case FLOAT:
      callback.f(atof(value));
      break;

    default:
      // bad valueType, do nothing
      break;
    }
  }
};


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * SerialController
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef SERIALCONTROLLER_MAX_CALLBACKS
#define SERIALCONTROLLER_MAX_CALLBACKS 16
#endif

// helper to define repetitious addCallback overloads
#define addCallbackDef(cbType, valType, u)				\
  void addCallback(const char *keyString, cbType callback) {		\
    SerialCallback *cb = callbacks + numCallbacks;			\
    strncpy(cb->key, keyString, MAX_KEY_LEN);			\
    cb->valueType = SerialCallback::ValueType::valType;			\
    cb->callback.u = callback;						\
    numCallbacks++;							\
    Serial.print("{added-callback:");					\
    Serial.print(keyString);						\
    Serial.println("}");						\
  }

class SerialController {
private:
  enum {
    WAIT_FOR_START,
    PARSE_KEY,
    PARSE_VALUE,
    N_PARSE_STATES
  } state;


  int numCallbacks;
  SerialCallback callbacks[SERIALCONTROLLER_MAX_CALLBACKS];
  char key[MAX_KEY_LEN];
  char value[MAX_VAL_LEN];
  bool steleProtocol;

  void waitForSerial(long baudrate) {
    Serial.begin(baudrate);
    while(!Serial);

    // wait for handshake if using steleProtocol
    while(!handshake && steleProtocol) {
      if (Serial.available() && Serial.read() == '{') {
	handshake = true;
	sendMessage("arduino-ready","1");
      }
    }
  }


  void lookupAndRunCallback() {
    if (steleProtocol && strcmp(key, "wake-arduino") == 0) {
      sendMessage("arduino-ready", "1");
      return;
    }

    for (int i=0; i<numCallbacks; i++) {
      sendMessage("check-callback", callbacks[i].key);
      if (strcmp(key, callbacks[i].key) == 0) {
	callbacks[i].respond(value);
	return;
      }
    }

    if (errorResponse) {
      sendMessage("unknown-command", key);
    }
  }

  
public:
  bool handshake;
  bool errorResponse;

  SerialController() { state = WAIT_FOR_START; numCallbacks = 0; }

  void setup(long baudrate=115200, bool steleProtocol=true) {
    this->steleProtocol = steleProtocol;
    errorResponse = true;
    waitForSerial(baudrate);
  }

  
  // overloaded callback-adding functions
  addCallbackDef(voidCallback, VOID, v)
  addCallbackDef(stringCallback, STRING, s)
  addCallbackDef(intCallback, INT, i)
  addCallbackDef(floatCallback, FLOAT, f)

  
  // message sending
  void sendMessage(char *messageKey, char *messageValue) {
    Serial.print("{");
    Serial.print(messageKey);
    Serial.print(":");
    Serial.print(messageValue);
    Serial.println("}");
  }

  void sendMessage(char *messageKey) {
    sendMessage(messageKey, "1");
  }

  void sendMessage(char *messageKey, int messageValue) {
    char value[MAX_VAL_LEN];
    snprintf(value, MAX_VAL_LEN, "%d", messageValue);
    sendMessage(messageKey, value);
  }


  void sendMessage(char *messageKey, float messageValue) {
    char value[MAX_VAL_LEN];
    snprintf(value, MAX_VAL_LEN, "%f", messageValue);
    sendMessage(messageKey, value);
  }


  // updating
  void update() {
    while (Serial.available() > 0) {
      char c = Serial.read();
      switch (state) {
      case WAIT_FOR_START:
	if (c == '{') {
	  memset(key, 0, MAX_KEY_LEN);
	  memset(value, 0, MAX_KEY_LEN);
	  state = PARSE_KEY;
	}
	break;

      case PARSE_KEY:
	if (c == '{') {
	  state = PARSE_KEY;
	}
	else if (c == ':') {
	  state = PARSE_VALUE;
	}
	else if (c == '}') {
	  // malformed input, look for next token
	  state = WAIT_FOR_START;
	}
	else {
	  strappend(key, c, MAX_KEY_LEN);
	}
	break;

      case PARSE_VALUE:
	if (c == '}') {
	  lookupAndRunCallback();
	  state = WAIT_FOR_START;
	}
	else {
	  strappend(value, c, MAX_VAL_LEN);
	}
	break;

      default:
	// something's gone wrong, reset
	state = WAIT_FOR_START;
	break;
      }
    }
  }
};

#endif
