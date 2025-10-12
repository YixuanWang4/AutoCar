//cpp file to store calculations regarding servo actions
#include <servocal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_Servo * servoD = Shield.getServo(0);
QGPMaker_Servo * servoU = Shield.getServo(2);
QGPMaker_Servo * servoR = Shield.getServo(4);
QGPMaker_Servo * servoF = Shield.getServo(6);

void calHandleDataServo(void * parameters){

    HandleData handleData;
    ServoSpeed servoSpeed;

    while(true){
        if(xSemaphoreTake(readHandleMutex, portMAX_DELAY) == pdPASS){
            handleData.l2 = ps2x.Button(PSB_L2);//Boolean
            handleData.r2 = ps2x.Button(PSB_R2);
            handleData.ry = ps2x.Analog(PSS_RY);
            handleData.green = ps2x.Button(PSB_GREEN);
            handleData.blue = ps2x.Button(PSB_BLUE);
            handleData.pink = ps2x.Button(PSB_PINK);
            handleData.red = ps2x.Button(PSB_RED);

            xSemaphoreGive(readHandleMutex);

            servoSpeed.cirSpd = map(handleData.ry, 0, 255, 3, -3);//-3 -> 3
            servoSpeed.rotSpd = ((int)handleData.r2 - (int)handleData.l2) * 4;
            servoSpeed.risSpd = ((int)handleData.green - (int)handleData.blue) * 3;
            servoSpeed.flawSpd = ((int)handleData.red - (int)handleData.pink) * 5;


            if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
                xQueueOverwrite(servoSpeedQueue, &servoSpeed);
            }

        }

        vTaskDelay(calServoDelay);

    }
}

void writeServoAngle(void * parameters){
    ServoSpeed servoSpeed;

    while(true){
        if(xQueueReceive(servoSpeedQueue, &servoSpeed, portMAX_DELAY) == pdPASS){

            servoD->writeServo(constrain(servoD->readDegrees() + servoSpeed.cirSpd, 0, 180));
            servoU->writeServo(constrain(servoU->readDegrees() - servoSpeed.cirSpd + servoSpeed.risSpd, 0, 180));
            servoR->writeServo(constrain(servoR->readDegrees() + servoSpeed.rotSpd, 0, 180));
            servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 180));

        }

        vTaskDelay(writeServoDelay);

    }

    vTaskDelete(NULL);

}