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
            handleData.rx = ps2x.Analog(PSS_RX);
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
            handleData.start = ps2x.ButtonReleased(PSB_START);
            xSemaphoreGive(readHandleMutex);

            if(handleData.start){
                xTaskCreate(autoLoad, "autoLoad", 2048, NULL, 2, NULL);
                vTaskSuspend(NULL);
            }

            servoSpeed.cirSpd = map(handleData.ry, 0, 255, 2, -2);//-3 -> 3
            servoSpeed.rotSpd = ((int)handleData.r2 - (int)handleData.l2) * 4;
            servoSpeed.risSpd = ((int)handleData.green - (int)handleData.blue) * 2;
            servoSpeed.flawSpd = ((int)handleData.red - (int)handleData.pink) * 3;
            servoSpeed.gndSpd = map(handleData.rx, 0, 255, -1, 1);
            if(handleData.padR3){servoSpeed.gndSpd = 2;}
            servoSpeed.storeMode = 0;

            
            if(handleData.padUp){
                servoSpeed.storeMode = 1;
            }else if(handleData.padRight){
                servoSpeed.storeMode = 2;
            }else if(handleData.padDown){
                servoSpeed.storeMode = 3;
            }else if(handleData.select){
                servoSpeed.storeMode = 4;
            }else if(handleData.padL3){
                servoSpeed.storeMode = 5;
            }else if(handleData.padLeft){
                servoSpeed.storeMode = 6;
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
    byte dAng = 135, uAng = 135, rAng = 0;

    while(true){
        if(xQueueReceive(servoSpeedQueue, &servoSpeed, portMAX_DELAY) == pdPASS){

            switch(servoSpeed.storeMode){
                case 0:
                    servoD->writeServo(constrain(servoD->readDegrees() + servoSpeed.cirSpd, 0, 180));
                    servoU->writeServo(constrain(servoU->readDegrees() - servoSpeed.cirSpd + servoSpeed.risSpd, 0, 180));
                    servoR->writeServo(constrain(servoR->readDegrees() + servoSpeed.rotSpd, 0, 180));
                    servoF->writeServo(constrain(servoF->readDegrees() + servoSpeed.flawSpd, 0, 133));
                    break;
                case 1://Big soldier
                    servoD->writeServo(75);//75
                    servoU->writeServo(22 - suOffset);//22
                    servoR->writeServo(74);//74
                    servoF->writeServo(45);
                    vTaskDelay(20);
                    break;
                case 2:
                    servoD->writeServo(64);//67
                    servoU->writeServo(36 - suOffset);//48
                    servoR->writeServo(74);//94
                    servoF->writeServo(45);
                    vTaskDelay(20);
                    break;
                case 3://Super Soldier
                    servoD->writeServo(59);//58
                    servoU->writeServo(44 - suOffset);//62
                    servoR->writeServo(74);//98
                    servoF->writeServo(45);
                    vTaskDelay(20);
                    break;
                case 4:
                    dAng = servoD->readDegrees();
                    uAng = servoU->readDegrees();
                    rAng = servoR->readDegrees();
                    Serial.println(dAng);Serial.println(uAng);Serial.println(rAng);

                    vTaskDelay(20);
                    break;
                case 5:
                
                    servoD->writeServo(dAng);
                    servoU->writeServo(uAng);
                    servoR->writeServo(rAng);

                    vTaskDelay(20);

                    break;
                case 6:
                    servoD->writeServo(64);//62
                    servoU->writeServo(13 - suOffset);//15
                    servoR->writeServo(0);//98
                    servoF->writeServo(45);

                    vTaskDelay(20);
                    break;

                default:
                    break;
            }
            /*
            if(servoSpeed.gndSpd != 2){
                servoGL->writeServo(constrain(servoGL->readDegrees() + servoSpeed.gndSpd, 0, 120));
                servoGR->writeServo(constrain(servoGR->readDegrees() - servoSpeed.gndSpd, 60, 180));
            }else{
                vTaskSuspend(servoHandle);
                servoGL->writeServo(170);
                vTaskDelay(1000);
                servoGR->writeServo(10);
                vTaskResume(servoHandle);
            }
            */
            
        }

        vTaskDelay(writeServoDelay);

    }

    vTaskDelete(NULL);

}

void autoLoad(void * parameters){
    ServoSpeed servoSpeed;

    servoSpeed.cirSpd = 0;
    servoSpeed.flawSpd = 0;
    servoSpeed.gndSpd = 0;
    servoSpeed.risSpd = 0;
    servoSpeed.rotSpd = 0;
    servoSpeed.storeMode = 4;//Init the struct,memorize the place
    if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
        xQueueOverwrite(servoSpeedQueue, &servoSpeed);
    }

    servoF->writeServo(45);
    vTaskDelay(500);//Release the first super soldier

    servoSpeed.storeMode = 3;
    if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
        xQueueOverwrite(servoSpeedQueue, &servoSpeed);
    }//Go for the upper soldier

    vTaskDelay(1100);

    servoF->writeServo(133);
    vTaskDelay(500);

    servoSpeed.storeMode = 5;
    if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
        xQueueOverwrite(servoSpeedQueue, &servoSpeed);
    }//Go to the memorized place

    vTaskDelay(1100);

    servoF->writeServo(45);
    vTaskDelay(500);//Release the second super soldier

    servoSpeed.storeMode = 6;
    if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
        xQueueOverwrite(servoSpeedQueue, &servoSpeed);
    }//Go for the lower soldier
    vTaskDelay(1100);

    servoF->writeServo(133);
    vTaskDelay(500);

    servoSpeed.storeMode = 5;
    if(xQueueSend(servoSpeedQueue, &servoSpeed, portMAX_DELAY) != pdPASS){
        xQueueOverwrite(servoSpeedQueue, &servoSpeed);
    }//Go to the memorized place

    vTaskDelay(1100);
    servoF->writeServo(45);
    vTaskDelay(500);//Release the second super soldier

    vTaskResume(servoHandle);
    vTaskDelete(NULL);
}