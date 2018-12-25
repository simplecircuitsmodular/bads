# Arcade Sequencer

## Features

This is a fairly simple trigger sequencer for Eurorack drums. It has an internal and external clock, 6 channels that can be trigered by the onboard buttons or external input jacks, pause/play functionality, recording for up to 64 steps, and the ability to set the pattern length to any multiple of 3 or 4 and 1, 2, 4, 8, or 16. I'll likely add a clear button and the ability to step through one step at a time. There's still 3 free pins on the Arduino so a few more features are possible.

## Files
### BADS.ino
This is the Arduino code. An ISP header on the board allows the microcontroller to be programmed directly.

### BADS_Schematic_xx.pdf
This is the schematic in .pdf format. Since there's no custom PCB for this build there's not eagle files either.

### BADSParts_xx.md
This is the parts list. A Mouser BOM is available at in this file as well.

### BADS.ai
This is the panel in Adobe Illustrator format. This can be used to manufacture a panel.

## Updates

### 01.1 07MAY18

Removed serial to allow D0 and D1 to be used as inputs. Added ability to clear all or clear individual channels by wiring a momentary toggle to D0, D1, and gnd. Switching to clear all immidiately clears all channels, switching to clear channel then triggering a channel clears that channel. Basic comments added.

### 01 03MAY18

First commit and working code. No comments, serial debugging stuff left in. Still rough, but funtional. There may be some unused variables.
