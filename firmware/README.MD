In this folder you will find the firmwares available for Cascadence.

They are all written in Arduino, and rely on tinySPI (https://github.com/JChristensen/tinySPI)

Locking Sequencer - a dual 4 step sequencer

Clock In moves the sequencer to the next step
The 4 potentiometers adjust the pitch of each step.
Switching the toggle to the left or right enables editing of that sequencer, while the other remains unchanged

ADSR - a dual analog modelling envelope
Simulates a capacitor charging and discharging at the attack, decay, and sustain stages by modelling a first-order filter

Clock in - trigger/gate to start both envelopes
Potentiometers from top to bottom - Attack, Decay, Sustain, Release
Toggle switch - changes active editing of envelope A or B.
