/*  This is the first version of autocar controller programme
    Comprehensive note will be included to better understand the code
    
    Authored by controller group
    Maintained by Baotian

*/

#include <Arduino.h> //To enable the basic Arduino function

#include <v10.h> //To include our header
#include <wheelcal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include "PS2X_lib.h" //To communicate with our handle

QueueHandle_t motorSpeedQueue;
SemaphoreHandle_t readHandleMutex;

TickType_t readHandleDelay = 10;//The rate of reading handle
TickType_t calHandleDelay = 10;//The rate of calculating data
TickType_t writeDelay = 10;//The rate of writing data to motor
float maxSpeed = 1000;//The maxspeed of minicar, default to 1000 mm/s
float angle = 0.6435011088;//Currently not designed, default to arctan(0.75)
float length = 0.25;//Currently not designed, default to 0.25
float radius = 0.03;//The radius of our wheel

void setup(){

    Serial.begin(9600); //Due to the cable, we cannot set a higher baud rate

    //initialize our Motorboard
    bool setUpResult = initShield();
    if (setUpResult != true){
        Serial.println("W1:Cannot initialize Motorboard!");
        while(true){
            vTaskDelay(50);//Since we cannot initialize pipe, we'll just stop here for check
        };
    };

    bool pipeInitResult = pipeInit();
    if(pipeInitResult != true){
        Serial.println("W2:Cannot initialize FreeRTOS pipe!");
        while(true){
            vTaskDelay(50);//Since we cannot initialize Motorboard, we'll just stop here for check
        };
    };

    xTaskCreate(getHandleData, "getHandleData", 4096, NULL, 1, NULL);
    xTaskCreate(calHandlePara, "calHandlePara", 4096, NULL, 1, NULL);
    xTaskCreate(writeMotorAngSpd, "writeMotorAngSpd", 4096, NULL, 1, NULL);

};//To initialize our autocar and set our functions

void loop(){

};//The loop function will do nothing, since work has been added to the rtos system

bool initShield(){
    
    Shield.begin(50);
    return true;//This ugly library cannot give response, so we'll return true anyway

};//initialize our Motorboard

bool pipeInit(){

    motorSpeedQueue = xQueueCreate(300, sizeof(MotorSpeed));
    if(motorSpeedQueue == NULL){
        Serial.println("W3:Cannot initialize queue!");
        return false;
    };

    readHandleMutex = xSemaphoreCreateMutex();
    if (readHandleMutex == NULL) {
        Serial.println("W4:Failed to create mutex!");
        return false;
    }

    return true;

};//init the pipe we'll use to transmit numbers