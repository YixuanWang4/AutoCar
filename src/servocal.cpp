//cpp file to store calculations regarding servo actions
#include <servocal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_Servo * servoD = Shield.getServo(1);
QGPMaker_Servo * servoU = Shield.getServo(2);
QGPMaker_Servo * servoR = Shield.getServo(3);
QGPMaker_Servo * servoF = Shield.getServo(4);

void calHandleDataServo(void * parameters){

    HandleData handleData;
    ServoSpeed servoSpeed;

    while(true){
        if(xSemaphoreTake(readHandleMutex, 5) == pdPASS){
            handleData.l2 = ps2x.Button(PSB_L2);//Boolean
            handleData.r2 = ps2x.Button(PSB_R2);
            handleData.ry = ps2x.Analog(PSS_LY);
            handleData.green = ps2x.Button(PSB_GREEN);
            handleData.blue = ps2x.Button(PSB_BLUE);

            xSemaphoreGive(readHandleMutex);

            servoSpeed.cirSpd = map(handleData.ry, 0, 255, 10, -10);
            servoSpeed.rotSpd = ((int)handleData.r2 - (int)handleData.l2) * 5;
            servoSpeed.flawSpd = ((int)handleData.green - (int)handleData.blue) * 5;

            if(xQueueSend(servoSpeedQueue, &servoSpeed, 2) != pdPASS){
                Serial.println("WW1:Queue was full!");
            }

        }

        vTaskDelay(calServoDelay);

    }
}

void writeServoAngle(void * parameters){
    
}