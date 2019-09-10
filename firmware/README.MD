In this folder you will find the firmwares available for Cascadence. If you wish to submit your own firmware please get in touch with me (fil@cctv.fm).

They are all written in Arduino, and rely on tinySPI (https://github.com/JChristensen/tinySPI)

## Locking Sequencer - a dual 4 step sequencer

Clock In moves the sequencer to the next step

The 4 potentiometers adjust the pitch of each step.

Switching the toggle to the left or right enables editing of that sequencer, while the other remains unchanged

## ADSR - a dual analog modelling envelope
Simulates a capacitor charging and discharging at the attack, decay, and release stages by modelling a first-order filter

Clock in - trigger/gate to start both envelopes

Potentiometers from top to bottom - Attack, Decay, Sustain, Release

Toggle switch - changes active editing of envelope A or B.

## Euclidean Sequencer
Generates mathematically derived rhythms from a set of input conditions. 32 step.

Clock in - Trigger input

Potentiometers from top to bottom:
1. Length - the total step length of the sequence from 0 to 32
2. Density - the number of pulses to distribute in the sequence, scales from 1 to Length
3. Offset - shifts the sequence right this number of steps. Scales from 1 to Length
4. Randomness - The likelyhood of a pulse state being inverted. At full counter-clockwise, 0 randomness is added. At full clockwise every pulse will be inverted (no pulse becomes pulse, pulse becomes no pulse). In between min and max, the sequence will change at every iteration. The 'coin' is flipped every time a pulse is about to be sent out.

Toggle Switch - changes active editing of sequence A or B.

## Turing Machine

Algorithmic Sequence generator with variable probability of mutation. Implemented here with a linked pulse output and optional quantizer.

Clock in - Trigger input

Potentiometers from top to bottom:
1. Randomness - this is the big knob on the original TM. Fully clockwise and your loop is repeated, fully counter clockwise, and your loop is inverted creating a mirrored loop twice as long as the 'length' setting. In the middle is a 50% chance of mutation at every iteration
2. Length - adjustable from 1 to 16. This is the number of bits in the shift register. The output is scaled to the bit length, and the output is taken from the available bits you have set here, for example if length is 3, you get a 3 bit output scaled to the full range of the output, limited by the 'Scale' setting.
3. Scale - the range from the lowest to the highest voltage outputed. Fully clockwise is the max range, fully counter clockwise you get no output.
4. Offset - Shifts the sequence up but preserving relationships

Toggle Switch - Engages a Chromatic Quantizer. Right is quantized, Left is unquantized.

A Output - The CV sequence output

B Output - Sends a pulse if the LSB in the shift register is '1'. You can use this in conjunction with the CV sequence to create interesting patterns.

## Template

A Simple template to get you started with writing your own Cascadence code. Includes the DAC functions and pin definitions so all the hard work is done for you.
