lighting-controller
===================

This sketch takes serial input dictating the current genome from the Mega and then updates the Soya DotStar array to match.


Protocol
--------

This sketch uses stele-style packets at 115200 baud for communication. It only accepts one key: "genome", the value of which should be a six-character string containing only 'G', 'A', 'T', or 'C'. This string will then be processed into a unique eye color layout.


Overview
--------

Almost all of the objects are contained in the global namespace and are managed by the main sketch more or less directly. The LEDs use the [FastLED library] and are defined by the main sketch before being passed by reference to the `disk` object.

  * `SerialController.h` - `arduino-base` boilerplate for stele packet communication
  * `SoyaDisk.h` - defines the `Ring` and `Disk` classes used for controlling the LEDs
  * `lighting-controller.ino` - main sketch entry point

[FastLED library]: http://fastled.io/
