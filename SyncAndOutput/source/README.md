![HDC logo](../../hidden_data_small.png)
VGA Sync Generator
==================

This directory contains the source code for a Arduino based VGA horizontal and vertical sync generator, which also generates the blanking period on another pin.

The code was developed and tested on a Arduino Nano, but should also work on an Uno.  It relies on precise timing and requires a 16MHz clock speed for the Arduino.  This is the default for Arduino Nanos so any unmodified Nano will work.

Building and uploading
----------------------

To build, [platform io](https://platformio.org/) is required.  In the `SyncAndOutput/source` directory (i.e. this directory) type:

* pio run -t upload

This will download the required platform and libraries, compile the `.hex` file and upload it to the Arduino.
