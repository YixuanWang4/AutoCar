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


void setup(){

    Serial.begin(9600); //Due to the cable, we cannot set a higher baud rate

    //initialize our Motorboard
    bool setUpResult = initShield();
    if (setUpResult != true){
        Serial.println("W1:Cannot initialize Motorboard!");
        while(true){
            //Since we cannot initialize pipe, we'll just stop here for check
        };
    };

    bool pipeInitResult = pipeInit();
    if(pipeInitResult != true){
        Serial.println("W2:Cannot initialize FreeRTOS pipe!");
        while(true){
            //Since we cannot initialize Motorboard, we'll just stop here for check
        };
    };

};//To initialize our autocar and set our functions

void loop(){

};//The loop function will do nothing, since work has been added to the rtos system

bool initShield(){
    
    Shield.begin(50);
    return true;//This ugly library cannot give response, so we'll return true anyway

};//initialize our Motorboard

bool pipeInit(){


    return true;
};//init the pipe we'll use to transmit numbers