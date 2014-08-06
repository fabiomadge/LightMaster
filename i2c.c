#include <xc.h>
#include <stdint.h>
#include "i2c.h"

#define I2C_SLAVE  0x2A

void sendByte(uint8_t);
void sendArray(const uint8_t [], uint8_t, uint8_t);
void I2CWait();
void I2CStart();
void I2CStop();
void sendBlob();

Machine m0 = {
    {1, 0x01, 0x01, 0x01, 8, 0, 25, 10},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0}
};

Machine m1 = {
    {1, 0x7F, 0x7F, 0x7F, 255, 0, 0, 0},
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0}
};

//1 coinituous led
Machine m2 = {
    {1, 0x01, 0x01, 0x01, 3, 173, 85, 0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0,  0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255,  0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0,  1},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0, 87}
};

//2 coinituous leds
Machine m3 = {
    {1, 0x7F, 0x7F, 0x7F, 3, 86, 85, 86},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0, 0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255, 0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255, 0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0, 0}
};

//3 coinituous leds
Machine m4 = {
    {1, 0x01, 0x01, 0x01, 3, 0, 85, 173},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0,  0},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255,  1},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255, 87},
    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255,  0}
};


Machine machines[10];

void configI2C(){
    TRISB5 = 1;
    TRISB7 = 1;

    SSP2CON1 = 0b00101000;
    SSP2ADD = 39; // 16000 kHz /(4 � (39 + 1)) = 100kHz
    SSP2STAT = 0b10000000; //disable Slewratecontrol

    machines[0] = m0;
    machines[1] = m1;
    machines[2] = m2;
    machines[3] = m3;
    machines[4] = m4;
}

void sendStateMachine(int mach){
    //sendBlob();
    uint8_t ar[] = {255};
    sendArray(ar, 1, 31);
    sendArray(machines[mach].global, 8, 32);
    sendArray(machines[mach].led0, 9, 64);
    sendArray(machines[mach].led1, 9, 80);
    sendArray(machines[mach].led2, 9, 96);
    sendArray(machines[mach].led3, 9, 112);
}

void updateColors(Colors c){
    uint8_t arr[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
    sendArray(c.led0, 6, 64);
    sendArray(c.led1, 6, 80);
    sendArray(c.led2, 6, 96);
    sendArray(c.led3, 6, 112);
}

void updateTimings(uint8_t ts[]){
    sendArray(ts, 3, 37);
}

void sendArray(const uint8_t arr[], uint8_t l, uint8_t offset){
    I2CStart();
    uint8_t payload = (0x00 | I2C_SLAVE);
    sendByte(payload); //Adress the LED Board
    sendByte(offset);
    for(int i = 0; i < l; i++){
        sendByte(arr[i]);
    }
    I2CStop();
}

void sendByte(uint8_t byte){
    SSP2BUF = byte; // Move data to SSPBUF
    I2CWait();

    if(SSP2CON2bits.ACKSTAT){  //Error; No Ack
        LATC =  0b0001111;
        while(1) continue;
    }
}

void I2CWait(){
    while(!SSP2IF) continue;
    SSP2IF = 0;
}

void I2CStart(){
    SSP2IF = 0;
    SSP2CON2bits.SEN = 1;  // Generate Start Condition
    I2CWait();
}

void I2CStop(){
    SSP2CON2bits.PEN = 1;// Generate Stop Condition
    I2CWait();
}