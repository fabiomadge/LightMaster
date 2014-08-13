/* 
 * File:   i2c.h
 * Author: fabio
 *
 * Created on July 2, 2014, 4:24 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>

typedef struct Machine{
    //{Enable, BrRed, BrGreen, BrBlue, StepSize, AtMin_Num, Ramp_Num, AtMax_Num}
    uint8_t global[8]; // 32 - 39
    //{MS_Red, LS_Red, MS_Green, LS_Green, MS_Blue, LS_Blue, State, LogDim, Count}
    uint8_t led0[9];   // 64 - 73
    uint8_t led1[9];   // 80 - 89
    uint8_t led2[9];   // 96 -105
    uint8_t led3[9];   // 112-121
    uint8_t counter;   // 31
    uint8_t config;    // 40
} Machine;

//State    enum{AT_MIN, RAMP_UP, AT_MAX, RAMP_DOWN}

typedef struct Colors{
    //{MS_Red, LS_Red, MS_Green, LS_Green, MS_Blue, LS_Blue}
    uint8_t led0[6];   // 64 - 70
    uint8_t led1[6];   // 80 - 86
    uint8_t led2[6];   // 96 -102
    uint8_t led3[6];   // 112-118
} Colors;

void configI2C();
void sendStateMachine(Machine);
void updateStateMachine(Machine);
void updateColors(Colors);
void updateTimings(uint8_t ts[]);

#endif	/* I2C_H */