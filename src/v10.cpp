/*  This is the first version of autocar controller programme
    Comprehensive note will be included to better understand the code
    
    Authored by controller group
    Maintained by Baotian， Riyueshiguang
*/

#include <Arduino.h>
#include <v10.h> //To include our header

//Variables definations

QueueHandle_t motorSpeedQueue;
SemaphoreHandle_t readHandleMutex;

TickType_t readHandleDelay = 8;//The rate of reading handle
TickType_t calHandleDelay = 20;//The rate of calculating data
TickType_t writeDelay = 5;//The rate of writing data to motor
float maxSpeed = 1000;//The maxspeed of minicar, default to 1000 mm/s
float angle = 0.7086;//Currently not designed, default to arctan(0.75)
float length = 0.136;//Currently not designed, default to 0.25
float radius = 0.03;//The radius of our wheel

//Main programme

void setup(){

    Serial.begin(9600); //Due to the cable, we cannot set a higher baud rate
    Serial.println("MiniCar initialize! Version:V1.1");

    for(int count = 0; count < 10; count ++){
        Serial.print('.');
        vTaskDelay(100);
    }
    
    //initialize our Motorboard
    initShield();

    bool pipeInitResult = pipeInit();
    if(!pipeInitResult){
        Serial.println("W2:Cannot initialize FreeRTOS pipe!");
        while(true){
            vTaskDelay(50);//Since we cannot initialize Motorboard, we'll just stop here for check
        }
    }

    xTaskCreate(getHandleData, "getHandleData", 2048, NULL, 1, NULL);
    xTaskCreate(calHandleParaWheel, "calHandleParaWheel", 2048, NULL, 2, NULL);
    xTaskCreate(writeMotorAngSpd, "writeMotorAngSpd", 2048, NULL, 3, NULL);

}//To initialize our autocar and set our functions

void loop(){

}//The loop function will do nothing, since work has been added to the rtos system

bool initShield(){
    
    Shield.begin(50);

    int error = ps2x.config_gamepad(18,23,5,19, true, true);
    if(error != 0){
        return false;
    }

    return true;

}//initialize our Motorboard

//Functions definations

bool pipeInit(){

    motorSpeedQueue = xQueueCreate(100, sizeof(MotorSpeed));
    if(motorSpeedQueue == NULL){
        Serial.println("W3:Cannot initialize queue!");
        return false;
    }

    readHandleMutex = xSemaphoreCreateMutex();
    if (readHandleMutex == NULL) {
        Serial.println("W4:Failed to create mutex!");
        return false;
    }

    return true;

}//init the pipe we'll use to transmit numbers