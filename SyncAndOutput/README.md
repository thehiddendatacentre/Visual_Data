![HDC logo](../hidden_data_small.png)
VGA Sync Generator
==================

This is a sync generator for a low cost 3 bit RGB-VGA video synthesizer.  It uses an [Arduino](https://www.arduino.cc/) microcontroller and two 74HC High-speed CMOS chips to generate a horizontal sync pulse, a vertical sync pulse and a blanking period pulse.

The source code relies on [platform io](https://platformio.org/) to build and upload to the Arduino.
Timings for the various syncing pulses are taken from [tinyvga](http://www.tinyvga.com/vga-timing/640x480@60Hz) and then converted to either Arduino cycles @ 16MHz, for the horizontal sync, or horizontal lines for the vertical sync.
