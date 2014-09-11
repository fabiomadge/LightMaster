#include <xc.h>
#include <stdint.h>
#include "i2c.h"

#define I2C_SLAVE  0x2A

#define GLOBAL_LENGTH  8  //38-31
#define LED_LENGTH     9  // sizeof(LED)
#define MACHINE_SIZE  44  // sizeof(Machine)
#define LED_COUNT      4

void sendByte(uint8_t);
void sendArray(const uint8_t [], uint8_t, uint8_t);
void I2CWait();
void I2CStart();
void I2CStop();

void computeUpdate(uint8_t * value, uint8_t * adrss, uint8_t * lastVal, uint8_t adress, const uint8_t * prev, const uint8_t * new);
void getLED(const LED * source, uint8_t * aim);
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

    //assemble array of global parameters 
    uint8_t globals[GLOBAL_LENGTH];
    globals[0] = last.Counter;
    globals[1] = last.Enable;
    globals[2] = last.StepSize;
    globals[3] = last.AtMin_NumFrames;
    globals[4] = last.Ramp_NumFrames;
    globals[5] = last.AtMax_NumFrames;
    globals[6] = getConfigByte(&last);
    globals[7] = last.UpdateDelay;

    //shells for the byte sized parameters
    uint8_t led0[LED_LENGTH];
    uint8_t led1[LED_LENGTH];
    uint8_t led2[LED_LENGTH];
    uint8_t led3[LED_LENGTH];

    //fill with information
    getLED(&(last.LEDs[0]), led0);
    getLED(&(last.LEDs[1]), led1);
    getLED(&(last.LEDs[2]), led2);
    getLED(&(last.LEDs[3]), led3);

    sendArray(globals, GLOBAL_LENGTH, MM_LED_COUNTER_ADDR);
    sendArray(led0, LED_LENGTH, MM_LED0_BASE_ADDR);
    sendArray(led1, LED_LENGTH, MM_LED1_BASE_ADDR);
    sendArray(led2, LED_LENGTH, MM_LED2_BASE_ADDR);
    sendArray(led3, LED_LENGTH, MM_LED3_BASE_ADDR);
}

//checks for delta to *last* and updates accordingly after updating *last*
void updateStateMachine(const Machine * m){
    uint8_t value[MACHINE_SIZE];
    uint8_t adrss[MACHINE_SIZE];

    uint8_t lastVal = 0;

    //asamble update list
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
    for(int i = 0; LED_COUNT; i++){
        uint8_t baseAddr = 64 + (i*16);

        uint8_t prvLed[LED_LENGTH];
        uint8_t newLed[LED_LENGTH];

        getLED(&(last.LEDs[i]), &prvLed);
        getLED(&((*m).LEDs[i]), &newLed);

        for(int j = 0; j < LED_LENGTH; j++){
            computeUpdate(value, adrss, &lastVal, baseAddr + j,  &(prvLed[j]),  &(newLed[j]));
        }
    }

    //work thew update list
    uint8_t done = 0;
    while(done < lastVal){
        uint8_t interval = 0;
        while((done + interval + 1) < lastVal && adrss[done+interval] == (adrss[done+interval+1]-1)) interval++;

        //account for first elem
        interval++;

        uint8_t valBuff[MACHINE_SIZE];
        for(int i = 0; i < interval; i++){
            valBuff[i] = value[done+i];
        }
        sendArray(valBuff, interval, value[done]);
        done+=interval;
    }

    //save machine
    last = *m;
}

//one transmission session according to doc
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

//extract to LED information and cuts into into byte size
void getLED(const LED * source, uint8_t * aim){
    aim[0] = (uint8_t) (0x00FF & ((*source).Brightness.Red   >> 8));
    aim[1] = (uint8_t) (0x00FF & ((*source).Brightness.Red   >> 0));
    aim[2] = (uint8_t) (0x00FF & ((*source).Brightness.Green >> 8));
    aim[3] = (uint8_t) (0x00FF & ((*source).Brightness.Green >> 0));
    aim[4] = (uint8_t) (0x00FF & ((*source).Brightness.Blue  >> 8));
    aim[5] = (uint8_t) (0x00FF & ((*source).Brightness.Blue  >> 0));
    aim[6] = (*source).State;
    aim[7] = (*source).LogDim;
    aim[8] = (*source).Count;
}

//generate config byte
uint8_t getConfigByte(const Machine * source){
    uint8_t val = 0;
    if((*source).Lock) val |= 0x01;
    if((*source).Up)   val |= 0x02;
    if((*source).Log)  val |= 0x04;
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
