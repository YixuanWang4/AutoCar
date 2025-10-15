/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_MotorShield Shield;

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
        }
        vTaskDelay(readHandleDelay);
    }

    vTaskDelete(NULL);
    
}//Get data form our handle

void calHandleParaWheel(void *pvParameters){

    HandleData handleData;
    CarStatus carStatus;
    MotorSpeed motorSpeed;

    while(true){

        if(xSemaphoreTake(readHandleMutex, portMAX_DELAY) == pdPASS){
            handleData.l1 = ps2x.Button(PSB_L1);//Boolean
            handleData.r1 = ps2x.Button(PSB_R1);
            handleData.lx = ps2x.Analog(PSS_LX);//Byte
            handleData.ly = ps2x.Analog(PSS_LY);
            handleData.select = ps2x.ButtonPressed(PSB_SELECT);
            xSemaphoreGive(readHandleMutex);

            if(handleData.select){
                if(maxSpeed > 750){
                    maxSpeed = 500;
                }else{
                    maxSpeed = 1000;
                }
            }

            carStatus.vy = (float)map(handleData.ly, 0, 255, -maxSpeed, maxSpeed) / 1000.0;
            carStatus.vx = (float)map(handleData.lx, 0, 255, -maxSpeed, maxSpeed) / 1000.0;
            carStatus.w = ((float)handleData.l1 - (float)handleData.r1) * M_PI * maxSpeed / 700.0;

            //Turning speed: pai (s^-1)

            motorSpeed.wFL = (carStatus.vx + carStatus.vy +
                            sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
            motorSpeed.wFR = (0 - carStatus.vx + carStatus.vy -
                            sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
            motorSpeed.wBL = (0 - carStatus.vx + carStatus.vy + 
                            sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;
            motorSpeed.wBR = (carStatus.vx + carStatus.vy - 
                            sqrt(2) * cos(M_PI_4 - angle) * carStatus.w * length) / radius;

            //Serial.print("wfl speed:");Serial.println(motorSpeed.wFL);

            if(xQueueSend(motorSpeedQueue, &motorSpeed, portMAX_DELAY) != pdPASS){
                xQueueOverwrite(motorSpeedQueue, &motorSpeed);
            }
        }

        vTaskDelay(calMotorDelay);
        
    }

    vTaskDelete(NULL);

}//Calculate angular speed from handle data

void writeMotorAngSpd(void *pvParameters){

    MotorSpeed motorSpeed;

    while(true){
        if(xQueueReceive(motorSpeedQueue, &motorSpeed, portMAX_DELAY) != pdPASS){

            Serial.println("WW2:Cannot receive data from queue!");

        }else{

            motorFL->setSpeed(map(constrain(abs(motorSpeed.wFL), 0, 85), 0, 85, 0, 255));
            motorFR->setSpeed(map(constrain(abs(motorSpeed.wFR), 0, 85), 0, 85, 0, 255));
            motorBL->setSpeed(map(constrain(abs(motorSpeed.wBL), 0, 85), 0, 85, 0, 255));
            motorBR->setSpeed(map(constrain(abs(motorSpeed.wBR), 0, 85), 0, 85, 0, 255));

            if(motorSpeed.wFL > 0.2){
                motorFL->run(FORWARD);
            }else if(motorSpeed.wFL < -0.2){
                motorFL->run(BACKWARD);
            }else{
                motorFL->run(RELEASE);
            }

            if(motorSpeed.wFR > 0.2){
                motorFR->run(FORWARD);
            }else if(motorSpeed.wFR < -0.2){
                motorFR->run(BACKWARD);
            }else{
                motorFR->run(RELEASE);
            }

            if(motorSpeed.wBL > 0.2){
                motorBL->run(FORWARD);
            }else if(motorSpeed.wBL < -0.2){
                motorBL->run(BACKWARD);
            }else{
                motorBL->run(RELEASE);
            }

            if(motorSpeed.wBR > 0.2){
                motorBR->run(FORWARD);
            }else if(motorSpeed.wBR < -0.2){
                motorBR->run(BACKWARD);
            }else{
                motorBR->run(RELEASE);
            }
        }

        vTaskDelay(writeDelay);

    }
}//Write speed order to the motor
