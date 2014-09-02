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
#include "manager.h"

#define MAXPHASE 13

/******************************************************************************/
/* User Global Variable Declaration and Prototypes                            */
/******************************************************************************/

void buttonWatch();
void shortPress();
void longPress();

uint8_t phase;
uint16_t pressed;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main()
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    phase = 0;
    pressed = 0;

    //Wait for initialization to finish
    uint32_t s = 0x0000FFFF;
    while(s != 0) s--;

    //main loop
    while (1) {
        buttonWatch();
    }

    return 0;

}

//polls the button and differentiates between long and short press
void buttonWatch(){
    static uint8_t recLong = 0;

    uint8_t s = 0xFF;
    while(s != 0) s--;

    //button pressed and won't flow over
    if(!PORTAbits.RA2 && pressed != 0xFFFF) pressed++;
    else{
        //was pressed before and there is no leftover from a longpress
        if(pressed > 0 && recLong == 0) shortPress();
        recLong = 0;
        pressed = 0;
    }

    //400ms
    if(pressed > 0x400){
        //long press
        longPress();
        pressed = 0;
        if(recLong < 0xFF) recLong++;
    }
}

//plays the currently selected animation
void shortPress(){
    playPhase(phase);
}

//selects new animation
void longPress(){
    phase+1 >= MAXPHASE ? phase = 0 : phase++;
    LATC = phase;
}