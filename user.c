/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"
#include "i2c.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    TRISC = 0;                                          //all pins are outputs

    //pot
    TRISAbits.TRISA4 = 1;
    ANSELAbits.ANSA4 = 1;

    //switch
    TRISAbits.TRISA2 = 1;
    ANSELAbits.ANSA2 = 0;

    ADCON0 = 0b00001101;
    ADCON1 = 0b00010000;

    LATC = 0b0000000;

    configI2C();
}

