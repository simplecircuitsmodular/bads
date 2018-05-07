# Big Ass Drum Sequencer

## Overview

This is a fairly simple trigger sequencer for Eurorack drums. It has an internal and external clock, 6 channels that can be trigered by the onboard buttons or external input jacks, pause/play functionality, recording for up to 64 steps, and the ability to set the pattern length to any multiple of 3 or 4 and 1, 2, 4, 8, or 16. I'll likely add a clear button and the ability to step through one step at a time. There's still 3 free pins on the Arduino so a few more features are possible.

## Materials

6x illuminated arcade switches

13x jacks, at least 1 of which must be switched

1x Arduino Nano

6x 10k resistor

6x schottky diode

1x potentiometer, 10k or so. Anything that keeps the current down is fine, but I ususally use 5-20k.

4x 2.2k resistor

1x 7805 regulator

1x 2x5 header

2x 100n capacitor

3x ON-ON switch

1x 5 position rotary switch

Stripboard

## Schematic
I'll be putting one together in Fritzing soon. 

In short: 
1. take a 10k resistor from pins D3-D8 to ground. 
2. Wire one side of the arcade button to +5v and the other to one of the pins with the resistors. 
3. Wire a jack to each pin with the diodes in between to protect your external source. 
4. Wire D9-13 and A0 to the positive LED in each button and to the tip of 6 jacks. 
5. Wire the sleeves of the jacks and the negative side of the LEDs to ground. 
6. Wire A2 to the wiper of your pot, 1 pin to gnd and 3 to +5. 
7. Wire the 2k2 resistors between lugs on the rotary switch, then wire 1 end to +5 and 1 to gnd. The common pin goes to A2. 
8. A3, A5, and A6 go to the common pin on the toggle switches. The other pins go to +5 and gnd. 
9. A4 goes to the switched lug of your jack. The unswitched side goes to D2, and wire the sleeve to gnd. 

You can change any of these pins as long as you change them in the code, but the clock needs to be on an interrupt.

## Code

BADS.ino. Written in Arduino 1.8.1. 

Changes:

03MAY18 v0.0: First commit and working code. No comments, serial debugging stuff left in. Still rough, but funtional. There may be some unused variables.
07MAY18: v0.0.1: Removed serial to allow D0 and D1 to be used as inputs. Added ability to clear all or clear individual channels by wiring a momentary toggle to D0, D1, and gnd. Switching to clear all immidiately clears all channels, switching to clear channel then triggering a channel clears that channel. Basic comments added.

## License
CC BY-SA 4.0. See LICENSE.md for full text.