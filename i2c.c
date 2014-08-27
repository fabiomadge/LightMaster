#include <xc.h>
#include <stdint.h>
#include "i2c.h"

#define I2C_SLAVE  0x2A

void sendByte(uint8_t);
void sendArray(const uint8_t [], uint8_t, uint8_t);
void I2CWait();
void I2CStart();
void I2CStop();

//const Machine m0 = {
//    {1, 0x01, 0x01, 0x01, 8, 0, 25, 10},
//    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
//    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
//    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0},
//    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1,  55, 0}
//};
//
//const Machine m1 = {
//    {1, 0x7F, 0x7F, 0x7F, 255, 0, 0, 0},
//    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
//    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
//    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0},
//    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 4,  0x01, 0}
//};
//
////1 coinituous led
//const Machine m2 = {
//    {1, 0x01, 0x01, 0x01, 3, 173, 85, 0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0,  0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255,  0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0,  1},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0, 87}
//};
//
////2 coinituous leds
//const Machine m3 = {
//    {1, 0x7F, 0x7F, 0x7F, 3, 86, 85, 86},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0, 0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255, 0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255, 0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0,  0, 0}
//};
//
////3 coinituous leds
//const Machine m4 = {
//    {1, 0x01, 0x01, 0x01, 3, 0, 85, 173},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 1,  0,  0},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255,  1},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 2,255, 87},
//    {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 3,255,  0}
//};
//
//Machine machines[10];

Machine last;

void configI2C(){
    TRISB5 = 1;
    TRISB7 = 1;

    SSP2CON1 = 0b00101000;
    SSP2ADD = 39; // 16000 kHz /(4 × (39 + 1)) = 100kHz
    SSP2STAT = 0b10000000; //disable Slewratecontrol

//    machines[0] = m0;
//    machines[1] = m1;
//    machines[2] = m2;
//    machines[3] = m3;
//    machines[4] = m4;
}

void sendStateMachine(Machine m){
    last = m;

    uint8_t ar[11];
    ar[0] = m.counter;
    for(uint8_t i = 0; i < 8; i++) ar[i+1] = m.global[i];
    ar[9] = m.config;
    ar[10] = m.updateDelay;

    sendArray(ar, 11, 31);
    sendArray(m.led0, 9, 64);
    sendArray(m.led1, 9, 80);
    sendArray(m.led2, 9, 96);
    sendArray(m.led3, 9, 112);
}

void updateStateMachine(Machine m){
    uint8_t val[47];
    uint8_t id[47];

    uint8_t length = 0;

    if(m.counter != last.counter){
        val[length] = m.counter;
        id[length]  = 31;
        last.counter = m.counter;
        length++;
    }

    for(int i = 0; i < 8; i++){
        if(m.global[i] != last.global[i]){
            val[length] = m.global[i];
            id[length]  = 32+i;
            last.global[i] = m.global[i];
            length++;
        }
    }

    if(m.config != last.config){
        val[length] = m.config;
        id[length]  = 40;
        last.config = m.config;
        length++;
    }

    if(m.updateDelay != last.updateDelay){
        val[length] = m.updateDelay;
        id[length]  = 41;
        last.updateDelay = m.updateDelay;
        length++;
    }

    uint8_t * lastLeds;
    uint8_t * newLeds;
    for(int i = 0; i < 4; i++){
        switch(i){
            case 0:{
                newLeds = m.led0;
                lastLeds = last.led0;
            } break;
            case 1:{
                newLeds = m.led1;
                lastLeds = last.led1;
            } break;
            case 2:{
                newLeds = m.led2;
                lastLeds = last.led2;
            } break;
            case 3:{
                newLeds = m.led3;
                lastLeds = last.led3;
            } break;
        }
        for(int j = 0; j < 9; j++){
            uint8_t newVal  = *(newLeds  + j);
            uint8_t lastVal = *(lastLeds + j);
            if(newVal != lastVal){
                val[length] = newVal;
                id[length]  = 64 + (i*16) + j;
                *(lastLeds + j) = newVal;
                length++;
            }
        }
    }

    uint8_t done = 0;
    while(done < length){
        uint8_t interval = 0;
        while((done + interval + 1) < length && id[done+interval] == (id[done+interval+1]-1)) interval++;

        //account for first elem
        interval++;

        uint8_t valBuff[47];
        for(int i = 0; i < interval; i++){
            valBuff[i] = val[done+i];
        }
        sendArray(valBuff, interval, id[done]);
        done+=interval;
    }
}

//void updateColors(Colors c){
//    const uint8_t arr[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
//    sendArray(c.led0, 6, 64);
//    sendArray(c.led1, 6, 80);
//    sendArray(c.led2, 6, 96);
//    sendArray(c.led3, 6, 112);
//}
//
//void updateTimings(uint8_t ts[]){
//    sendArray(ts, 3, 37);
//}

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
        while(1){
            uint16_t s = 0xFFFF;
            while(s != 0) s--;
            LATC =  0b0000000;
            s = 0xFFFF;
            while(s != 0) s--;
            LATC =  0b0001111;
        };
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