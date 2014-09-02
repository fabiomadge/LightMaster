# LightMaster

Sample I2C master implementation for testing PIC software controling a LED ring.

## Setup

The sources are delivered in the form of a MPLAB X project. Building should be as easy as hitting build.

## Hardware

The software is tested to work with the PIC16LF1829. It should work on similar processors.

##Usage

A short press dispatches the currently selected animation. (red LEDs)

By pressing the button for a longer time, every ≈400ms the next animation is selcated. This should be visible in the red leds counting up in binary.

## Structure

The [main loop](../master/main.c) decides by watching to button, when and what animation to play. It passes this information to the [*manager.c*](../master/manager.c) which holds the acutal state machine. It is send and updated by using the functions offered by [*i2c.c*](../master/i2c.c).
