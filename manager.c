#include "i2c.h"

void on();
void off();
void pairing();
void play();
void pause();
void volumeUp();
void volumeDown();
void batteryLow();
void batteryCritical();
void charging();

void davewheel();
void onewheel();
void twowheel();
void threewheel();

void disco();
void strobe();

void clean(Machine *);
void global(uint8_t g[], const uint8_t a[]);

void playPhase(int i){
    switch(i){
        case 0:  on(); break;
        case 1:  pairing(); break;
        case 2:  play(); break;
        case 3:  batteryLow(); break;
        case 4:  batteryCritical(); break;
        case 5:  pause(); break;
        case 6:  off(); break;
        case 7:  charging(); break;
        case 8:  davewheel(); break;
        case 9:  onewheel(); break;
        case 10: twowheel(); break;
        case 11: threewheel(); break;
        case 12: disco(); break;
    }
}

//white pulses
void on(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 5, 10, 51, 15};
    global((mach.global), glob);
    for(int i = 2; i < 4; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 54;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

void off(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 5, 10, 51, 15};
    global((mach.global), glob);
    for(int i = 0; i < 2; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 54;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

//blue cross
void pairing(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 5, 0, 51, 0};
    global((mach.global), glob);
    for(int i = 4; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 102;
    mach.config = 0b00100000;
    mach.updateDelay = 54;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led1[6] = 2;
    mach.led1[7] = 255;

    mach.led3[6] = 2;
    mach.led3[7] = 255;


    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b11100000;
    updateStateMachine(mach);

    for(uint32_t i = 0; i < 0x000FFFFF; i++) continue;

    mach.counter = 0x39;

    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 4; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
}


void play(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 15, 255, 17, 0};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x7F;
    mach.config = 0b00100000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 3;   break; //state
            case 7: val = 255; break; //dimm
            case 8: val = 0;   break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x0004FFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

void pause(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 3, 255, 85, 0};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 44;
    mach.config = 0b00100000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 3;  break; //state
            case 7: val = 255 break; //dimm
            case 8: val = 0;  break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x0004FFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

//is currently in a testing branch of the led board pic
void volumeUp(){
}

//is currently in a testing branch of the led board pic
void volumeDown(){
}

//two orange pulses
void batteryLow(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 5, 0, 51, 0};
    global((mach.global), glob);
    for(int i = 0; i < 4; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    for(int i = 2; i < 3; i++){
        mach.led0[i] = 0x7F;
        mach.led1[i] = 0x7F;
        mach.led2[i] = 0x7F;
        mach.led3[i] = 0x7F;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 24;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x000CFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

//four short red pulses
void batteryCritical(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x09, 0x09, 0x09, 5, 0, 51, 0};
    global((mach.global), glob);
    for(int i = 0; i < 2; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 16;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x000EFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

void charging(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x39, 0x39, 0x39, 3, 173, 85, 0};
    global((mach.global), glob);
    for(int i = 0; i < 2; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00000000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led0[6] = 1;

    mach.led1[6] = 3;
    mach.led1[7] = 255;

    mach.led2[8] = 1;

    mach.led3[8] = 87;

    sendStateMachine(mach);

    while(mach.led0[2] != 0xFF){
        for(int i = 2; i < 4; i++){
            mach.led0[i]++;
            mach.led1[i]++;
            mach.led2[i]++;
            mach.led3[i]++;
        }

        for(uint32_t i = 0; i < 0x00000FFF; i++) continue;

        updateStateMachine(mach);

    }

    while(mach.led0[0] != 0x00){
        for(int i = 0; i < 2; i++){
            mach.led0[i]--;
            mach.led1[i]--;
            mach.led2[i]--;
            mach.led3[i]--;
        }

        for(uint32_t i = 0; i < 0x00000FFF; i++) continue;

        updateStateMachine(mach);

    }

    mach.config = 0b11000000;
    updateStateMachine(mach);
}

void davewheel(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x39, 0x39, 0x39, 8, 0, 25, 10};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 64;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 1;   break; //state
            case 7: val = 175; break; //dimm
            case 8: val = 0;   break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led0[7] = 55;

    mach.led1[8] = 15;

    mach.led2[6] = 2;
    mach.led2[7] = 255;
    mach.led2[8] = 5;

    mach.led3[6] = 3;
    mach.led3[8] = 10;

    sendStateMachine(mach);
}

void onewheel(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x39, 0x39, 0x39, 3, 173, 85, 0};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00000000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led0[6] = 1;

    mach.led1[6] = 3;
    mach.led1[7] = 255;

    mach.led2[8] = 1;

    mach.led3[8] = 87;

    sendStateMachine(mach);
}

void twowheel(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x39, 0x39, 0x39, 3, 86, 85, 86};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00000000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 0; break; //state
            case 7: val = 0; break; //dimm
            case 8: val = 0; break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led0[6] = 1;

    mach.led1[6] = 2;
    mach.led1[7] = 255;

    mach.led2[6] = 3;
    mach.led2[7] = 255;

    sendStateMachine(mach);
}

void threewheel(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x39, 0x39, 0x39, 3, 0, 85, 173};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00000000;
    mach.updateDelay = 8;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 2;   break; //state
            case 7: val = 255; break; //dimm
            case 8: val = 0;   break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    mach.led0[6] = 1;
    mach.led0[7] = 0;

    mach.led1[8] = 1;

    mach.led2[8] = 87;

    mach.led3[6] = 3;

    sendStateMachine(mach);
}

void disco(){
    strobe();
}

void strobe(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x7F, 0x7F, 0x7F, 255, 64, 1, 0};
    global((mach.global), glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00100000;
    mach.updateDelay = 0;
    for(uint8_t i = 6; i < 9; i++){
        uint8_t val = 0;
        switch(i){
            case 6: val = 2;   break; //state
            case 7: val = 255; break; //dimm
            case 8: val = 0;   break; //count
        }
        mach.led0[i] = val;
        mach.led1[i] = val;
        mach.led2[i] = val;
        mach.led3[i] = val;
    }

    sendStateMachine(mach);
    
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    mach.counter = 0;
    mach.config = 0b10100000;
    updateStateMachine(mach);
}

//enshures a defined state for the machine
void clean(Machine * m){
    m->config = 0;
    m->counter = 0;
    m->updateDelay = 0;
    for(int i = 0; i < 8; i++) m->global[i] = 0;
    for(int i = 0; i < 9; i++){
        m->led0[i] = 0;
        m->led1[i] = 0;
        m->led2[i] = 0;
        m->led3[i] = 0;
    }
}

//overwrites the content of  array g with the ones of array a
void global(uint8_t g[], const uint8_t a[]){
    for(int i = 0; i < 8; i++) g[i] = a[i];
}