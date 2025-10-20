//cpp file to store calculations regarding servo actions
#include <servocal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

QGPMaker_Servo * servoD = Shield.getServo(0);
QGPMaker_Servo * servoU = Shield.getServo(2);
QGPMaker_Servo * servoR = Shield.getServo(4);
QGPMaker_Servo * servoF = Shield.getServo(6);
QGPMaker_Servo * servoGL = Shield.getServo(3);
QGPMaker_Servo * servoGR = Shield.getServo(5);

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
            handleData.padLeft = ps2x.Button(PSB_PAD_LEFT);
            handleData.padRight = ps2x.Button(PSB_PAD_RIGHT);
            handleData.padUp = ps2x.Button(PSB_PAD_UP);
            handleData.padDown = ps2x.Button(PSB_PAD_DOWN);
            handleData.start = ps2x.Button(PSB_START);
            handleData.padL3 = ps2x.Button(PSB_L3);
            handleData.padR3 = ps2x.Button(PSB_R3);
            handleData.select = ps2x.Button(PSB_SELECT);

            xSemaphoreGive(readHandleMutex);

            servoSpeed.cirSpd = map(handleData.ry, 0, 255, 3, -3);//-3 -> 3
            servoSpeed.rotSpd = ((int)handleData.r2 - (int)handleData.l2) * 4;
            servoSpeed.risSpd = ((int)handleData.green - (int)handleData.blue) * 3;
            servoSpeed.flawSpd = ((int)handleData.red - (int)handleData.pink) * 3;
            servoSpeed.gndSpd = ((int)handleData.padRight - (int)handleData.padLeft) * 2;
            servoSpeed.start = handleData.start;
            servoSpeed.storeMode = 0;

            
            if(handleData.padUp){
                servoSpeed.storeMode = 1;
            }else if(handleData.padDown){
                servoSpeed.storeMode = 2;
            }else if(handleData.padL3){
                servoSpeed.storeMode = 3;
            }else if(handleData.select){
                servoSpeed.storeMode = 4;
            }else if(handleData.padR3){
                servoSpeed.storeMode = 5;
            }
            

            if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
                xQueueOverwrite(servoSpeedQueue, &servoSpeed);
            }

        }

        vTaskDelay(calServoDelay);

    }
}

void writeServoAngle(void * parameters){
    ServoSpeed servoSpeed;
    byte dAng = 135, uAng = 135, rAng = 10;

    servoGR->writeServo(135);
    vTaskDelay(1500);
    servoGL->writeServo(45);

    while(true){
        if(xQueueReceive(servoSpeedQueue, &servoSpeed, portMAX_DELAY) == pdPASS){

            switch(servoSpeed.storeMode){
                case 0:
                    servoD->writeServo(constrain(servoD->readDegrees() + servoSpeed.cirSpd, 0, 180));
                    servoU->writeServo(constrain(servoU->readDegrees() - servoSpeed.cirSpd + servoSpeed.risSpd, 0, 180));
                    servoR->writeServo(constrain(servoR->readDegrees() + servoSpeed.rotSpd, 0, 180));
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 135));
                    break;
                case 1://Little soldier
                    servoD->writeServo(78);//78
                    servoU->writeServo(34);//34
                    servoR->writeServo(98);//98
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 135));
                    vTaskDelay(20);
                    break;
                case 2:
                    servoD->writeServo(67);//67
                    servoU->writeServo(48);//48
                    servoR->writeServo(94);//94
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 135));
                    vTaskDelay(20);
                    break;
                case 3://Super Soldier
                    servoD->writeServo(58);//58
                    servoU->writeServo(62);//62
                    servoR->writeServo(98);//98
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 135));
                    vTaskDelay(20);
                    break;
                case 4:
                    dAng = servoD->readDegrees();
                    uAng = servoU->readDegrees();
                    rAng = servoR->readDegrees();
               
                    vTaskDelay(20);
                    break;
                case 5:
                    servoD->writeServo(dAng);
                    servoU->writeServo(uAng);
                    servoR->writeServo(rAng);

                    vTaskDelay(20);
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 135));

                    break;
                default:
                    break;
            }
            
            servoGL->writeServo(constrain(servoGL->readDegrees() + servoSpeed.gndSpd, 0, 120));
            servoGR->writeServo(constrain(servoGR->readDegrees() - servoSpeed.gndSpd, 60, 180));

            
        }

        vTaskDelay(writeServoDelay);

    }

    vTaskDelete(NULL);

}