# antennaduino
Arduino code for controlling remote antenna switch

This arduino code runs in a control box that switches antennas at a remote point on the roof.

The control box has six buttons, each with an LED, and four relays that switch the corresponding four relays in the remote switch. The possible settings are:

* Antenna 1: No relays energized; default setting on startup or selected by pressing button 1.
* Antenna 2 (north): R1 and R2 are energized, selecting  the phased pair directed north.
* Antenna 2 (omni): R1, R2, and R4 are energized (R3 doesn't matter in this setting), selecting the pair fed in phase.
* Antenna 2 (south) R1, R2, and R3 are energized, selecting the phased pair directed south.
* Antenna 3: R1 is energized.

The setting is selected by pressing the appropriate button, which illuminates in a modified radiobutton scheme.
