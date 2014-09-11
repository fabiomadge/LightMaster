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

//green pulses
void on(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          5;
    mach.AtMin_NumFrames =  10;
    mach.Ramp_NumFrames =   51;
    mach.AtMax_NumFrames =  15;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      54;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0x0000;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0x0000;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
}

//red pulses
void off(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          5;
    mach.AtMin_NumFrames =  10;
    mach.Ramp_NumFrames =   51;
    mach.AtMax_NumFrames =  15;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      54;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0x0000;
        mach.LEDs[i].Brightness.Blue  = 0x0000;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
}

//blue cross
void pairing(){
    Machine mach;
    clean(&mach);

    mach.Counter =         102;
    mach.Enable =            1;
    mach.StepSize =          5;
    mach.AtMin_NumFrames =   0;
    mach.Ramp_NumFrames =   51;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      54;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0x0000;
        mach.LEDs[i].Brightness.Green = 0x0000;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Count   = 0;

        if(i%2 == 0){
            mach.LEDs[i].State   = 0;
            mach.LEDs[i].LogDim  = 0;
        }
        else{
            mach.LEDs[i].State   =   2;
            mach.LEDs[i].LogDim  = 255;
        }
    }

    sendStateMachine(&mach);

    //wait for successful pairing
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    //light up all leds
    mach.Counter = 0;
    mach.Lock = 1;
    mach.Up = 1;
    mach.Log = 1;

    updateStateMachine(&mach);

    for(uint32_t i = 0; i < 0x000FFFFF; i++) continue;

    //turn it all down again
    mach.counter = 0x39;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State  =   4;
        mach.LEDs[i].LogDim =   0;
        mach.LEDs[i].Count  =   0;
    }

    sendStateMachine(mach);
}

//slow ramp up followed by quick ramp down
void play(){
    Machine mach;
    clean(&mach);

    mach.Counter =        0x7F;
    mach.Enable =            1;
    mach.StepSize =         15;
    mach.AtMin_NumFrames = 255;
    mach.Ramp_NumFrames =   17;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   =   3;
        mach.LEDs[i].LogDim  = 255;
        mach.LEDs[i].Count   =   0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x0004FFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
}

//quick ramp up followed by slow ramp down
void pause(){
    Machine mach;
    clean(&mach);

    mach.Counter =          44;
    mach.Enable =            1;
    mach.StepSize =          3;
    mach.AtMin_NumFrames = 255;
    mach.Ramp_NumFrames =   85;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   =   3;
        mach.LEDs[i].LogDim  = 255;
        mach.LEDs[i].Count   =   0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x0004FFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
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

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          5;
    mach.AtMin_NumFrames =   0;
    mach.Ramp_NumFrames =   51;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      24;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0x0000;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x000CFFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
}

//four short red pulses
void batteryCritical(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          5;
    mach.AtMin_NumFrames =   0;
    mach.Ramp_NumFrames =   51;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      16;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0x0000;
        mach.LEDs[i].Brightness.Blue  = 0x0000;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x000CFFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);
}

void charging(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          3;
    mach.AtMin_NumFrames = 173;
    mach.Ramp_NumFrames =   85;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               0;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0x0000;
        mach.LEDs[i].Brightness.Blue  = 0x0000;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    mach.LEDs[0].State = 1;
    mach.LEDs[1].State = 3;
    mach.LEDs[1].LogDim = 255;
    mach.LEDs[2].Count = 1;
    mach.LEDs[3].Count = 87;

    sendStateMachine(&mach);

    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    //fix the dimm level to the bottom
    mach.Counter = 0;
    mach.Lock =    1;
    mach.Up =      0;

    updateStateMachine(&mach);

    //fade to orange
    while(mach.LEDs[0].Brightness.Green != 0xFFFF){
        mach.LEDs[0].Brightness.Green += 0xFF;
        mach.LEDs[1].Brightness.Green += 0xFF;
        mach.LEDs[2].Brightness.Green += 0xFF;
        mach.LEDs[3].Brightness.Green += 0xFF;

        for(uint32_t i = 0; i < 0x00000FFF; i++) continue;

        updateStateMachine(&mach);
    }

    //fade to green
    while(mach.LEDs[0].Brightness.Red != 0x0000){
        mach.LEDs[0].Brightness.Red -= 0xFF;
        mach.LEDs[1].Brightness.Red -= 0xFF;
        mach.LEDs[2].Brightness.Red -= 0xFF;
        mach.LEDs[3].Brightness.Red -= 0xFF;

        for(uint32_t i = 0; i < 0x00000FFF; i++) continue;

        updateStateMachine(&mach);
    }

    //stop spinning
    mach.Lock = 1;
    mach.Up =   1;

    updateStateMachine(&mach);
}

void davewheel(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          8;
    mach.AtMin_NumFrames =   0;
    mach.Ramp_NumFrames =   25;
    mach.AtMax_NumFrames =  10;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =      64;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   =   1;
        mach.LEDs[i].LogDim  = 175;
        mach.LEDs[i].Count   =   0;
    }

    mach.LEDs[0].LogDim =  55;
    mach.LEDs[1].Count =   15;
    mach.LEDs[2].State =    2;
    mach.LEDs[2].LogDim = 255;
    mach.LEDs[2].Count =    5;
    mach.LEDs[3].State =    3;
    mach.LEDs[3].Count =   10;


    sendStateMachine(&mach);
}

void onewheel(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          3;
    mach.AtMin_NumFrames = 173;
    mach.Ramp_NumFrames =   85;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               0;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    mach.LEDs[0].State =    1;
    mach.LEDs[1].State =    3;
    mach.LEDs[1].LogDim = 255;
    mach.LEDs[2].Count =    1;
    mach.LEDs[3].Count =   87;


    sendStateMachine(&mach);
}

void twowheel(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          3;
    mach.AtMin_NumFrames =  86;
    mach.Ramp_NumFrames =   85;
    mach.AtMax_NumFrames =  86;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               0;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    mach.LEDs[0].State =    1;
    mach.LEDs[1].State =    2;
    mach.LEDs[1].LogDim = 255;
    mach.LEDs[2].State =    3;
    mach.LEDs[2].LogDim = 255;

    sendStateMachine(&mach);
}

void threewheel(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =          3;
    mach.AtMin_NumFrames =   0;
    mach.Ramp_NumFrames =   85;
    mach.AtMax_NumFrames = 173;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               0;
    mach.UpdateDelay =       8;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   = 0;
        mach.LEDs[i].LogDim  = 0;
        mach.LEDs[i].Count   = 0;
    }

    mach.LEDs[0].LogDim = 1;
    mach.LEDs[0].State =  0;
    mach.LEDs[1].Count =  1;
    mach.LEDs[2].Count = 87;
    mach.LEDs[3].State =  3;

    sendStateMachine(&mach);
}

void disco(){
    strobe();
}

void strobe(){
    Machine mach;
    clean(&mach);

    mach.Counter =           0;
    mach.Enable =            1;
    mach.StepSize =        255;
    mach.AtMin_NumFrames =  64;
    mach.Ramp_NumFrames =    1;
    mach.AtMax_NumFrames =   0;
    mach.Lock =              0;
    mach.Up =                0;
    mach.Log =               1;
    mach.UpdateDelay =       0;

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].Brightness.Red   = 0xFFFF;
        mach.LEDs[i].Brightness.Green = 0xFFFF;
        mach.LEDs[i].Brightness.Blue  = 0xFFFF;
    }

    for(int i = 0; i < LED_COUNT; i++){
        mach.LEDs[i].State   =   2;
        mach.LEDs[i].LogDim  = 255;
        mach.LEDs[i].Count   =   0;
    }

    sendStateMachine(&mach);

    //keep it from making you crazy
    for(uint32_t i = 0; i < 0x001FFFFF; i++) continue;

    mach.Lock = 1;    

    updateStateMachine(&mach);
}

//enshures a defined state for the machine
void clean(Machine * m){
    m->Counter = 0;
    m->Enable = 0;
    m->StepSize = 0;
    m->AtMin_NumFrames = 0;
    m->Ramp_NumFrames = 0;
    m->AtMax_NumFrames = 0;
    m->Lock = 0;
    m->Up = 0;
    m->Log = 0;
    m->UpdateDelay;
    for(int i = 0; i < LED_COUNT; i++){
        (*m).LEDs[i].Brightness.Red   = 0x0000;
        (*m).LEDs[i].Brightness.Green = 0xFFFF;
        (*m).LEDs[i].Brightness.Blue  = 0x0000;
        (*m).LEDs[i].State   = 0;
        (*m).LEDs[i].LogDim  = 0;
        (*m).LEDs[i].Count   = 0;
    }
}


//overwrites the content of  array g with the ones of array a
void global(uint8_t g[], const uint8_t a[]){
    for(int i = 0; i < 5; i++) g[i] = a[i];
}