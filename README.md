
# BLDC Motor Driver

This repository houses the comprehensive design for BLDC motor driver in Eagle for Maxon EC45 flat BLDC motors and program for electrical commutation

#### Board Design and Schematic:

 - Motor Driver: Custom 3-phase H-bridge using CMOS ICs (IRF7389) and Gate drivers
 - Motor: Maxon EC45 Brushless DC Motor (8 pins: Vhall, GND, Hall sensors 1-3, Phases 1-3)
 - The custom Motor driver has 6 Logic inputs (2 for each Phase of 3 Phase H bridge), 18V, GND and 5V for current sensor.
 - The driver has 3 output for each phase and output for current sensor.
 - The gate driver TC4426 (In previous design IR4426) is Dual High speed MOSFET driver, which can drive CMOS, the OUTPUT high is VDD - 0.025 V, thus it can easily switch PMOS.

#### Embedded Code:

 - Logic for commutation is written in AVR C for faster code implementation
 - Change in logic state of Hall sensor triggers an interrupt 
 - Based on 3 Hall sensor state logic input for each MOSFET is calculated using boolean function determined by Karnaugh Map
 - PORT and resister manipulation is done using bit shifting and bitwise boolean operation

## Connections

* Motor 3 Phases are connected to output 3 Phases of motor driver
* Hall sensor are connected to Digital Pin 2,3 and 8 (PORT D)
* Logic Input to Motor driver are connected to A0-A5 (PORT B)
* External Switch and push button can be attached to the pin 5 & 6 for direction control and to apply brake respectively.