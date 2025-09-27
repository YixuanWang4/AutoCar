/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>
#include <v10.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_MotorShield Shield;

QGPMaker_Servo * servoL;
QGPMaker_Servo * servoR;

QGPMaker_DCMotor * motorFL;
QGPMaker_DCMotor * motorBL;
QGPMaker_DCMotor * motorFR;
QGPMaker_DCMotor * motorBR;

PS2X ps2x;


void getHandleData(void *pvParameters){

    while(true){

        if(xSemaphoreTake(readHandleMutex, portMAX_DELAY) == pdPASS){
            ps2x.read_gamepad(false, 0);

            xSemaphoreGive(readHandleMutex);
        };

        

        vTaskDelay(readHandleDelay);
    };

    vTaskDelete(NULL);

};//Get data form our handle

void calHandlePara(void *pvParameters){

    HandleData handleData;
    CarStatus carStatus;
    MotorSpeed motorSpeed;

    while(true){

        if(xSemaphoreTake(readHandleMutex, portMAX_DELAY) == pdPASS){
            handleData.l1 = ps2x.Button(PSB_L1);//Boolean
            handleData.l2 = ps2x.Button(PSB_R2);
            handleData.r1 = ps2x.Button(PSB_R1);
            handleData.r2 = ps2x.Button(PSB_R2);

            handleData.lx = ps2x.Analog(PSS_LX);//Byte
            handleData.ly = ps2x.Analog(PSS_LY);
            handleData.rx = ps2x.Analog(PSS_RX);
            handleData.ry = ps2x.Analog(PSS_RY);

            xSemaphoreGive(readHandleMutex);

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
        };  

        if(xQueueSend(motorSpeedQueue, &motorSpeed, 0) != pdPASS){
            Serial.println("WW1:Queue was full!");
        };

        vTaskDelay(calHandleDelay);
        
    };

    vTaskDelete(NULL);

};//Calculate angular speed from handle data

void writeMotorAngSpd(void *pvParameters){

    MotorSpeed motorSpeed;

    while(true){
        if(xQueueReceive(motorSpeedQueue, &motorSpeed, portMAX_DELAY) != pdPASS){
            Serial.println("WW2:Cannot receive data from queue!");
        };

        motorFL->setSpeed(map(constrain(abs(motorSpeed.wFL), 0, 921), 0, 921.5, 0, 256));
        motorFR->setSpeed(map(constrain(abs(motorSpeed.wFR), 0, 921), 0, 921.5, 0, 256));
        motorBL->setSpeed(map(constrain(abs(motorSpeed.wBL), 0, 921), 0, 921.5, 0, 256));
        motorBR->setSpeed(map(constrain(abs(motorSpeed.wBR), 0, 921), 0, 921.5, 0, 256));

        if(motorSpeed.wFL > 0){
            motorFL->run(FORWARD);
        }else{
            motorFL->run(BACKWARD);
        };

        if(motorSpeed.wFR > 0){
            motorFR->run(FORWARD);
        }else{
            motorFR->run(BACKWARD);
        };

        if(motorSpeed.wBL > 0){
            motorBL->run(FORWARD);
        }else{
            motorBL->run(BACKWARD);
        };

        if(motorSpeed.wBR > 0){
            motorBR->run(FORWARD);
        }else{
            motorBR->run(BACKWARD);
        };

        vTaskDelay(writeDelay);

    };
};//Write speed order to the motor