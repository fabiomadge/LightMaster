/* 
 * File:   i2c.h
 * Author: fabio
 *
 * Created on July 2, 2014, 4:24 PM
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define LED_COUNT      4

// -- Our memory mapped I2C control registers address space definitions --

#define MM_LED_COUNTER_ADDR          (31)
#define MM_LED_ENABLE_ADDR           (32)
#define MM_LED_STEPSIZE_ADDR         (33)
#define MM_LED_ATMIN_NUMFRAMES_ADDR  (34)
#define MM_LED_RAMP_NUMFRAMES_ADDR   (35)
#define MM_LED_ATMAX_NUMFRAMES_ADDR  (36)
#define MM_LED_CONFIG_ADDR           (37)
#define MM_LED_UPDATE_DELAY_ADDR     (38)

#define MM_LED0_BASE_ADDR            (64)
#define MM_LED1_BASE_ADDR            (80)
#define MM_LED2_BASE_ADDR            (96)
#define MM_LED3_BASE_ADDR            (112)

#define MM_LEDX_RED_MS_OFFSET        (0)
#define MM_LEDX_RED_LS_OFFSET        (1)
#define MM_LEDX_GREEN_MS_OFFSET      (2)
#define MM_LEDX_GREEN_LS_OFFSET      (3)
#define MM_LEDX_BLUE_MS_OFFSET       (4)
#define MM_LEDX_BLUE_LS_OFFSET       (5)
#define MM_LEDX_STATE_OFFSET         (6)
#define MM_LEDX_LOGDIM_OFFSET        (7)
#define MM_LEDX_COUNT_OFFSET         (8)

typedef enum {
    AT_MIN,
    RAMP_UP,
    AT_MAX,
    RAMP_DOWN,
    END
} LED_SeqStates;

typedef struct RGB{
    uint16_t Red;   //0..1
    uint16_t Green; //2..3
    uint16_t Blue;  //4..5
} RGB;

typedef struct LED{
    RGB           Brightness; //0..5
    LED_SeqStates State;      //6
    uint8_t       LogDim;     //7
    uint8_t       Count;      //8
} LED;

typedef struct Machine {
    uint8_t Counter;         //31
    uint8_t Enable;          //32
    uint8_t StepSize;        //33
    uint8_t AtMin_NumFrames; //34
    uint8_t Ramp_NumFrames;  //35
    uint8_t AtMax_NumFrames; //36
    uint8_t Lock:1;          //37.7
    uint8_t Up:1;            //37.6
    uint8_t Log:1;           //37.5
    uint8_t :5;              //37.{4..0}
    uint8_t UpdateDelay;     //38
    LED     LEDs[4];         //64..72, 80..88, 96..104, 112..120
} Machine;

void configI2C();
void sendStateMachine(const Machine * m);
void updateStateMachine(const Machine * m);

#endif  /* I2C_H */