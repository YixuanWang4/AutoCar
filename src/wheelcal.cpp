/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>
#include <v10.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

void getHandleData(void){

    while(true){
        ps2x.read_gamepad(false, 0);

        vTaskDelay(readHandleDelay);
    };

    vTaskDelete(NULL);

};//Get data form our handle

void calHandlePara(void){

    HandleData handleData;
    CarStatus carStatus;
    MotorSpeed motorSpeed;

    while(true){
        handleData.l1 = ps2x.Button(PSB_L1);//Boolean
        handleData.l2 = ps2x.Button(PSB_R2);
        handleData.r1 = ps2x.Button(PSB_R1);
        handleData.r2 = ps2x.Button(PSB_R2);

        handleData.lx = ps2x.Analog(PSS_LX);//Byte
        handleData.ly = ps2x.Analog(PSS_LY);
        handleData.rx = ps2x.Analog(PSS_RX);
        handleData.ry = ps2x.Analog(PSS_RY);

        carStatus.vy = (float)map(handleData.ly, 0, 256, maxSpeed, -maxSpeed) / 1000.0;
        carStatus.vx = (float)map(handleData.lx, 0, 256, -maxSpeed, maxSpeed) / 1000.0;
        carStatus.w = ((float)handleData.r2 - (float)handleData.l2) * M_PI / 4.0;
        //Turning speed: pai/4 (s^-1)

        motorSpeed.wFL = (carStatus.vx + carStatus.vy +
                          sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wFR = (-carStatus.vx + carStatus.vy -
                          sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wBL = (-carStatus.vx + carStatus.vy + 
                          sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wBR = (carStatus.vx + carStatus.vy - 
                          sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;

        if(xQueueSend(motorSpeedQueue, &motorSpeed, 0) != pdPASS){
            Serial.println("WW1:Queue was full!");
        };

        vTaskDelay(calHandleDelay);
        
    };

    vTaskDelete(NULL);

};//Calculate angular speed from handle data

void writeMotorAngSpd(void){

};//Write speed order to the motor