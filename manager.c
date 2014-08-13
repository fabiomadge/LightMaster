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

void clean(Machine *);
void global(Machine *, const uint8_t []);

void playPhase(int i){
    switch(i){
        case 0: on(); break;
        case 1: pairing(); break;
        case 2: play(); break;
        case 3: volumeUp(); break;
        case 4: volumeDown(); break;
        case 5: batteryLow(); break;
        case 6: batteryCritical(); break;
        case 7: pause(); break;
        case 8: off(); break;
        case 9: charging(); break;
    }
}

void on(){
    Machine mach;
    clean(&mach);
    const uint8_t glob[] = {1, 0x01, 0x01, 0x01, 1, 0, 255, 255};
    global((mach.global), &glob);
    for(int i = 0; i < 6; i++){
        mach.led0[i] = 0xFF;
        mach.led1[i] = 0xFF;
        mach.led2[i] = 0xFF;
        mach.led3[i] = 0xFF;
    }
    mach.counter = 0x00;
    mach.config = 0b00000000;
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

//    for(uint32_t i = 0; i < 0xFFFFFFFF; i++) continue;

    //mach.config = 0b10100000;
//    mach.led0[2] = 0;
//    sendStateMachine(mach);

    //mach.
}

void off(){
}

void pairing(){
}

void play(){
}

void pause(){
}

void volumeUp(){

}

void volumeDown(){

}

void batteryLow(){

}

void batteryCritical(){

}

void charging(){

}

void clean(Machine * m){
    m->config = 0;
    m->counter = 0;
    for(int i = 0; i < 8; i++) m->global[i] = 0;
    for(int i = 0; i < 9; i++){
        m->led0[i] = 0;
        m->led1[i] = 0;
        m->led2[i] = 0;
        m->led3[i] = 0;
    }
}

void global(Machine * m, const uint8_t a[]){
    for(int i = 0; i < 8; i++) m->global[i] = a[i];
}