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
    uint8_t global[8];  // 32 - 39
    //{MS_Red, LS_Red, MS_Green, LS_Green, MS_Blue, LS_Blue, State, LogDim, Count}
    uint8_t led0[9];    // 64 - 73
    uint8_t led1[9];    // 80 - 89
    uint8_t led2[9];    // 96 -105
    uint8_t led3[9];    // 112-121
    uint8_t counter;    // 31
    uint8_t config;     // 40
    uint8_t updateDelay;// 41
} Machine;

void configI2C();
void sendStateMachine(Machine);
void updateStateMachine(Machine);

#endif	/* I2C_H */