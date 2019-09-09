In this folder you will find the firmwares available for Cascadence.

They are all written in Arduino, and rely on tinySPI (https://github.com/JChristensen/tinySPI)

## Locking Sequencer - a dual 4 step sequencer

Clock In moves the sequencer to the next step

The 4 potentiometers adjust the pitch of each step.

Switching the toggle to the left or right enables editing of that sequencer, while the other remains unchanged

## ADSR - a dual analog modelling envelope
Simulates a capacitor charging and discharging at the attack, decay, and sustain stages by modelling a first-order filter

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
