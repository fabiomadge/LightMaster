/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "i2c.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

void forth();
void back();
void wait();
void CalcUp();

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main()
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    LATC =  0b0000101;

    uint32_t s = 0x0000FFFF;
    while(s != 0) s--;

    LATC =  0b0001010;

    sendStateMachine(4);

    LATC =  0b0001111;

    LATC =  0b0001000;                                   //start the rotation by setting DS4 ON - rotate from the right to left

    while (1) {
        forth();
        back();
    }

    return 0;

}

void forth(){
    LATC =  0b0001000;
    while(!STATUSbits.C){
        wait();
        LATC >> = 1;
    }
}

void back(){
    LATC =  0b0000010;
    while(LATCbits.LATC3 == 0){
        wait();
        LATC << = 1;
    }
}

void wait(){
//    for(uint8_t i = 0xFF; i > 0; i--) continue;
//    GO = 1;
//    while (GO) continue;
//    unsigned short s = ((ADRESH << 8) | (ADRESL)) >> 0;
//
//    while(s != 0) s--;

    uint16_t s = 0xFFFF;
    while(s != 0){
        if(s & 0x00FF == 0x00FF) CalcUp();
        s--;
    };
}

void CalcUp(){
    static uint8_t prev = 0x00;

    for(uint8_t i = 0xFF; i > 0; i--) continue;
    GO = 1;
    while (GO) continue;

    if(prev != ADRESH){
        if(prev == 0xFF && ADRESH != 0xFF) sendStateMachine(0);
        if(prev != 0xFF && ADRESH == 0xFF) sendStateMachine(1);
        prev = ADRESH;
        Colors col = {
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
        };
        
        col.led0[0] = 0xFF - prev;
        col.led1[0] = 0xFF - prev;
        col.led2[0] = 0xFF - prev;
        col.led3[0] = 0xFF - prev;

        col.led0[2] = prev;
        col.led1[2] = prev;
        col.led2[2] = prev;
        col.led3[2] = prev;

        updateColors(col);
    }

}