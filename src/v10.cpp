/*  This is the first version of autocar controller programme
    Comprehensive note will be included to better understand the code
    
    Authored by controller group
    Maintained by Baotian， Riyueshiguang
*/

#include <v10.h> //To include our header

//Main programme

void setup(){

    Serial.begin(9600); //Due to the cable, we cannot set a higher baud rate
    Serial.println("MiniCar! Version:V1.2");

    //initialize our Motorboard
    initShield();
    servoInit();

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
    xTaskCreate(calHandleDataServo, "calHandleDataServo", 2048, NULL, 2, NULL);
    xTaskCreate(writeServoAngle, "writeServoAngle", 2048, NULL, 3, NULL);

}//To initialize our autocar and set our functions

void loop(){

}//The loop function will do nothing, since work has been added to the rtos system

bool initShield(){
    
    Shield.begin(50);

    motorFL = Shield.getMotor(1);
    motorBL = Shield.getMotor(4);
    motorFR = Shield.getMotor(2);
    motorBR = Shield.getMotor(3);

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
        Serial.println("W31:Cannot initialize motor queue!");
        return false;
    }

    servoSpeedQueue = xQueueCreate(100, sizeof(ServoSpeed));
    if(servoSpeedQueue == NULL){
        Serial.println("W32:Cannot initialize servo queue!");
        return false;
    }

    readHandleMutex = xSemaphoreCreateMutex();
    if (readHandleMutex == NULL) {
        Serial.println("W4:Failed to create mutex!");
        return false;
    }

    return true;

}//init the pipe we'll use to transmit numbers

bool servoInit(){

    //servoD->writeServo(45);
    servoD->writeServo(135);
    //servoU->writeServo(45 + suOffset);
    servoU->writeServo(135 + suOffset);
    servoR->writeServo(0);
    servoF->writeServo(0);
    servoGL->writeServo(170);
    vTaskDelay(1500);
    servoGR->writeServo(10);
    return true;
}