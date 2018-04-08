# antennaduino
Arduino code for controlling remote antenna switch

## Overview

This arduino code runs in a control box that switches antennas at a remote point on the roof.

The control box has six buttons, each with an LED, and four relays that switch the corresponding four relays in the remote switch. The possible settings are:

* Antenna 1: No relays energized; default setting on startup or selected by pressing button 1.
* Antenna 2 (north): R1 and R2 are energized, selecting  the phased pair directed north.
* Antenna 2 (omni): R1, R2, and R4 are energized (R3 doesn't matter in this setting), selecting the pair fed in phase.
* Antenna 2 (south) R1, R2, and R3 are energized, selecting the phased pair directed south.
* Antenna 3: R1 is energized.

The setting is selected by pressing the appropriate button, which illuminates in a modified radiobutton scheme.

## Physical housing

This is the initial build of the controller box:

![](antennaduino_exterior.png) ![](antennaduino_interior.png)

The Arduino reads the buttons, sets the LEDs, and controls the adjacent relays. These four relays in turn control 12v lines that run to the remote box to actuate the corresponding four relays there.

## Button actuation

Pressing any of the buttons generates an interrupt, which is handled as follows:

Press button | Light LEDs | Energize relays
-------------|------------|----------------
1 | 1 | (none)
2 | 2 and the last selected of 4/5/6 | R1 and R2, conditionally R3 and/or R4 as last selected
3 | 3 | R1
4 | 2 and 4 | R1, R2
5 | 2 and 5 | R1, R2, R3, R4
6 | 2 and 6 | R1, R2, R3
