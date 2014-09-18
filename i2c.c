#include <xc.h>
#include <stdint.h>
#include "i2c.h"

#define I2C_SLAVE  0x2A

#define GLOBAL_LENGTH  8  //38-31
#define LED_LENGTH     9  // sizeof(LED)
#define MACHINE_SIZE  44  // sizeof(Machine)
#define LED_COUNT      4

void sendByte(uint8_t);
void I2CWait();
void I2CStart();
void I2CStop();

void computeUpdate(uint8_t * value, uint8_t * adrss, uint8_t * lastVal, uint8_t adress, const uint8_t * prev, const uint8_t * new);
void sendGlobals(const Machine * source);
void sendLED(const LED * source, uint8_t offset);
uint8_t getLED(const LED * source, uint8_t byteNo);
uint8_t getConfigByte(const Machine * source);

Machine last;

void configI2C(){
    TRISB5 = 1;
    TRISB7 = 1;

    SSP2CON1 = 0b00101000;
    SSP2ADD = 39; // 16000 kHz /(4 × (39 + 1)) = 100kHz
    SSP2STAT = 0b10000000; //disable Slewratecontrol
}

//sends every byte in the machine to the slave and saves it in *last*
void sendStateMachine(const Machine * m){
    //save machine
    last = *m;

    sendGlobals(m);
    sendLED(&(last.LEDs[0]), MM_LED0_BASE_ADDR);
    sendLED(&(last.LEDs[1]), MM_LED1_BASE_ADDR);
    sendLED(&(last.LEDs[2]), MM_LED2_BASE_ADDR);
    sendLED(&(last.LEDs[3]), MM_LED3_BASE_ADDR);
}

//checks for delta to *last* and updates accordingly after updating *last*
void updateStateMachine(const Machine * m){
    uint8_t value[MACHINE_SIZE];
    uint8_t adrss[MACHINE_SIZE];

    uint8_t lastVal = 0;

    //assemble update list
    computeUpdate(value, adrss, &lastVal, MM_LED_COUNTER_ADDR,         &(last.Counter),         &(m->Counter));
    computeUpdate(value, adrss, &lastVal, MM_LED_ENABLE_ADDR,          &(last.Enable),          &(m->Enable));
    computeUpdate(value, adrss, &lastVal, MM_LED_STEPSIZE_ADDR,        &(last.StepSize),        &(m->StepSize));
    computeUpdate(value, adrss, &lastVal, MM_LED_ATMIN_NUMFRAMES_ADDR, &(last.AtMin_NumFrames), &(m->AtMin_NumFrames));
    computeUpdate(value, adrss, &lastVal, MM_LED_RAMP_NUMFRAMES_ADDR,  &(last.Ramp_NumFrames),  &(m->Ramp_NumFrames));
    computeUpdate(value, adrss, &lastVal, MM_LED_ATMAX_NUMFRAMES_ADDR, &(last.AtMax_NumFrames), &(m->AtMax_NumFrames));

    //construct config bytes
    uint8_t oldConf = getConfigByte(&last);
    uint8_t newConf = getConfigByte(m);
    computeUpdate(value, adrss, &lastVal, MM_LED_CONFIG_ADDR, &(oldConf), &(newConf));

    computeUpdate(value, adrss, &lastVal, MM_LED_UPDATE_DELAY_ADDR, &(last.UpdateDelay), &(m->UpdateDelay));

    //add led updates to the list
    for(int i = 0; i < LED_COUNT; i++){
        uint8_t baseAddr = 64 + (i*16);
        uint8_t prvByte;
        uint8_t newByte;

        for(int j = 0; j < LED_LENGTH; j++){
            prvByte = getLED(&(last.LEDs[i]), j);
            newByte = getLED(&((*m).LEDs[i]), j);

            computeUpdate(value, adrss, &lastVal, baseAddr + j,  &(prvByte),  &(newByte));
        }
    }

    //work through update list
    uint8_t done = 0;
    while(done < lastVal){
        uint8_t interval = 0;
        while((done + interval + 1) < lastVal && adrss[done+interval] == (adrss[done+interval+1]-1)) interval++;

        //account for first elem
        interval++;

        //send array
        I2CStart();
        uint8_t payload = (0x00 | I2C_SLAVE);
        sendByte(payload); //Adress the LED Board
        sendByte(adrss[done]);
        for(int i = 0; i < interval; i++){
            sendByte(value[done+i]);
        }
        I2CStop();

        done+=interval;
    }

    //save machine
    last = *m;
}

// //one transmission session according to doc
// void sendArray(const uint8_t arr[], uint8_t l, uint8_t offset){
//     I2CStart();
//     uint8_t payload = (0x00 | I2C_SLAVE);
//     sendByte(payload); //Adress the LED Board
//     sendByte(offset);
//     for(int i = 0; i < l; i++){
//         sendByte(arr[i]);
//     }
//     I2CStop();
// }

//send single byte
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

//wait for i2c interrupt flag to turn off
void I2CWait(){
    while(!SSP2IF) continue;
    SSP2IF = 0;
}

//start i2c session
void I2CStart(){
    SSP2IF = 0;
    SSP2CON2bits.SEN = 1;  // Generate Start Condition
    I2CWait();
}

//end i2c session
void I2CStop(){
    SSP2CON2bits.PEN = 1;// Generate Stop Condition
    I2CWait();
}

//extract to LED information and returns it
uint8_t getLED(const LED * source, uint8_t byteNo){
    switch(byteNo){
        case 0: return (uint8_t) (0x00FF & ((*source).Brightness.Red   >> 8));
        case 1: return (uint8_t) (0x00FF & ((*source).Brightness.Red   >> 0));
        case 2: return (uint8_t) (0x00FF & ((*source).Brightness.Green >> 8));
        case 3: return (uint8_t) (0x00FF & ((*source).Brightness.Green >> 0));
        case 4: return (uint8_t) (0x00FF & ((*source).Brightness.Blue  >> 8));
        case 5: return (uint8_t) (0x00FF & ((*source).Brightness.Blue  >> 0));
        case 6: return (*source).State;
        case 7: return (*source).LogDim;
        case 8: return (*source).Count;
        default: return 0;
    }
}


//extracts the infromation for one LED and sends it over i2c
void sendLED(const LED * source, uint8_t offset){
    I2CStart();
    uint8_t payload = (0x00 | I2C_SLAVE);
    sendByte(payload); //Adress the LED Board
    sendByte(offset);
    for(int i = 0; i < LED_LENGTH; i++){
        sendByte(getLED(source, i));
    }
    I2CStop();
}

//extracts the infromation for the globals and sends it over i2c
void sendGlobals(const Machine * source){
    I2CStart();
    uint8_t payload = (0x00 | I2C_SLAVE);
    sendByte(payload); //Adress the LED Board
    sendByte(MM_LED_COUNTER_ADDR);
    for(int i = 0; i < GLOBAL_LENGTH; i++){
        switch(i){
            case 0: payload = (*source).Counter; break;
            case 1: payload = (*source).Enable; break;
            case 2: payload = (*source).StepSize; break;
            case 3: payload = (*source).AtMin_NumFrames; break;
            case 4: payload = (*source).Ramp_NumFrames; break;
            case 5: payload = (*source).AtMax_NumFrames; break;
            case 6: payload = getConfigByte(source); break;
            case 7: payload = (*source).UpdateDelay; break;
        }
        sendByte(payload);
    }
    I2CStop();
}

//generate config byte
uint8_t getConfigByte(const Machine * source){
    uint8_t val = 0;
    if((*source).Lock) val |= 0x80;
    if((*source).Up)   val |= 0x40;
    if((*source).Log)  val |= 0x20;
    return val;
}

//compares the data and adds it to the update buffer if necessary
void computeUpdate(uint8_t * value, uint8_t * adrss, uint8_t * lastVal, uint8_t adress, const uint8_t * prev, const uint8_t * new){
    if((*prev) != (*new)){
        value[*lastVal] = (*new);
        adrss[*lastVal] = adress;
        (*lastVal)++;
    }
}