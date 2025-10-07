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

        if(xSemaphoreTake(readHandleMutex, 3) == pdPASS){
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

        if(xSemaphoreTake(readHandleMutex, 5) == pdPASS){
            handleData.l2 = ps2x.Button(PSB_L2);//Boolean
            handleData.r2 = ps2x.Button(PSB_R2);
            handleData.lx = ps2x.Analog(PSS_LX);//Byte
            handleData.ly = ps2x.Analog(PSS_LY);

            xSemaphoreGive(readHandleMutex);

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
        }

        vTaskDelay(calHandleDelay);
        
    }

    vTaskDelete(NULL);

}//Calculate angular speed from handle data

void writeMotorAngSpd(void *pvParameters){

    MotorSpeed motorSpeed;

    while(true){
        if(xQueueReceive(motorSpeedQueue, &motorSpeed, 5) != pdPASS){

            Serial.println("WW2:Cannot receive data from queue!");

        }else{

            motorFL->setSpeed(map(constrain(abs(motorSpeed.wFL), 0, 87), 0, 87, 10, 255));
            motorFR->setSpeed(map(constrain(abs(motorSpeed.wFR), 0, 87), 0, 87, 10, 255));
            motorBL->setSpeed(map(constrain(abs(motorSpeed.wBL), 0, 87), 0, 87, 10, 255));
            motorBR->setSpeed(map(constrain(abs(motorSpeed.wBR), 0, 87), 0, 87, 10, 255));

            if(motorSpeed.wFL > 0.5){
                motorFL->run(FORWARD);
            }else if(motorSpeed.wFL < -0.5){
                motorFL->run(BACKWARD);
            }else{
                motorFL->run(RELEASE);
            }

            if(motorSpeed.wFR > 0.5){
                motorFR->run(FORWARD);
            }else if(motorSpeed.wFR < -0.5){
                motorFR->run(BACKWARD);
            }else{
                motorFR->run(RELEASE);
            }

            if(motorSpeed.wBL > 0.5){
                motorBL->run(FORWARD);
            }else if(motorSpeed.wBL < -0.5){
                motorBL->run(BACKWARD);
            }else{
                motorBL->run(RELEASE);
            }

            if(motorSpeed.wBR > 0.5){
                motorBR->run(FORWARD);
            }else if(motorSpeed.wBR < -0.5
            
            
            ){
                motorBR->run(BACKWARD);
            }else{
                motorBR->run(RELEASE);
            }
        }

        vTaskDelay(writeDelay);

    }
};//Write speed order to the motor