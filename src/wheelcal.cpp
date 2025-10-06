/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_MotorShield Shield;

QGPMaker_DCMotor * motorFL = Shield.getMotor(1);
QGPMaker_DCMotor * motorBL = Shield.getMotor(3);
QGPMaker_DCMotor * motorFR = Shield.getMotor(2);
QGPMaker_DCMotor * motorBR = Shield.getMotor(4);

PS2X ps2x;


void getHandleData(void *pvParameters){

    while(true){

        if(xSemaphoreTake(readHandleMutex, 2) == pdPASS){
            ps2x.read_gamepad(false, 0);

            xSemaphoreGive(readHandleMutex);
        }
        vTaskDelay(readHandleDelay);
    }
    vTaskDelete(NULL);
};//Get data form our handle

void calHandleParaWheel(void *pvParameters){

    HandleData handleData;
    CarStatus carStatus;
    MotorSpeed motorSpeed;

    while(true){

        if(xSemaphoreTake(readHandleMutex, 2) == pdPASS){
            handleData.l2 = ps2x.Button(PSB_L2);//Boolean
            handleData.r2 = ps2x.Button(PSB_R2);
            handleData.lx = ps2x.Analog(PSS_LX);//Byte
            handleData.ly = ps2x.Analog(PSS_LY);

            xSemaphoreGive(readHandleMutex);
        }

        //Serial.print("lx");Serial.println(handleData.lx);

        carStatus.vy = (float)map(handleData.ly, 0, 255, maxSpeed, -maxSpeed) / 1000.0;
        carStatus.vx = (float)map(handleData.lx, 0, 255, -maxSpeed, maxSpeed) / 1000.0;
        carStatus.w = ((float)handleData.r2 - (float)handleData.l2) * M_PI ;

        //Turning speed: pai/4 (s^-1)

        motorSpeed.wFL = (carStatus.vx + carStatus.vy +
                        sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wFR = (0 - carStatus.vx + carStatus.vy -
                        sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wBL = (0 - carStatus.vx + carStatus.vy + 
                        sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
        motorSpeed.wBR = (carStatus.vx + carStatus.vy - 
                        sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;

        //Serial.print("wfl speed:");Serial.println(motorSpeed.wFL);

        if(xQueueSend(motorSpeedQueue, &motorSpeed, 2) != pdPASS){
            Serial.println("WW1:Queue was full!");
        }

        vTaskDelay(calHandleDelay);
        
    }

    vTaskDelete(NULL);

}//Calculate angular speed from handle data

void writeMotorAngSpd(void *pvParameters){

    MotorSpeed motorSpeed;

    while(true){
        if(xQueueReceive(motorSpeedQueue, &motorSpeed, 2) != pdPASS){
            Serial.println("WW2:Cannot receive data from queue!");
        };

        motorFL->setSpeed(map(constrain(abs(motorSpeed.wFL), 0, 87), 0, 87, 0, 225));
        motorFR->setSpeed(map(constrain(abs(motorSpeed.wFR), 0, 87), 0, 87, 0, 225));
        motorBL->setSpeed(map(constrain(abs(motorSpeed.wBL), 0, 87), 0, 87, 0, 225));
        motorBR->setSpeed(map(constrain(abs(motorSpeed.wBR), 0, 87), 0, 87, 0, 225));

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

    }
};//Write speed order to the motor