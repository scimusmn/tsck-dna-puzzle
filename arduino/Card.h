#ifndef SMM_RFID_CARD_H
#define SMM_RFID_CARD_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Card
{
public:
    enum Status {
        READING,
        READ_OK,
        READ_BAD };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void addChar(char c)
    {
        if (buffer_index < 2) {
            buffer[buffer_index] = c;
            buffer_index++;
        }
        if (buffer_index == 2) {
            buffer_index = 0;
            processBuffer();
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    enum Status getStatus() { return status; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void printId()
    {
        for (int i=0; i<5; i++) {
            Serial.print(id[i], HEX);
            Serial.print(' ');
        }
        Serial.println();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void clear()
    {
        memset(buffer, 0, 3*sizeof(char));
        memset(id,     0, 5*sizeof(unsigned char));
        checksum = 0;

        id_index = 0;
        buffer_index = 0;

        status = READING;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    String getId()
    {
        char buf[15];
        snprintf(buf, 15*sizeof(char),
                 "%02x %02x %02x %02x %02x",
                 id[0], id[1], id[2], id[3], id[4]);
        return String(buf);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
    unsigned char id[5];
    char buffer[3];
    unsigned char checksum;
    
    unsigned int id_index;
    unsigned int buffer_index;
    enum Status status;

    void processBuffer()
    {
        unsigned long int n = strtoul(buffer, NULL, 16);
        if (id_index < 5) {
            id[id_index] = (unsigned char) (n & 0xff);
            id_index++;
        }
        else {
            checksum = n;
            checkId();
            id_index = 0;
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void checkId()
    {
        unsigned char sum = 0;
        for (int i=0; i<5; i++)
            sum = sum^id[i];

        if (sum == checksum)
            status = READ_OK;
        else
            status = READ_BAD;
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
