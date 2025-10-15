#ifndef _servocal
#define _servocal

#include <Arduino.h>
#include <wheelcal.h>

extern SemaphoreHandle_t readHandleMutex;
extern QueueHandle_t servoSpeedQueue;

extern TickType_t calServoDelay;
extern TickType_t writeServoDelay;
extern int servoRotateTime;

extern QGPMaker_Servo * servoD;
extern QGPMaker_Servo * servoU;
extern QGPMaker_Servo * servoR;
extern QGPMaker_Servo * servoF;
extern QGPMaker_Servo * servoGL;
extern QGPMaker_Servo * servoGR;

typedef struct{
    int cirSpd, rotSpd, flawSpd, risSpd, gndSpd, storeMode;
    bool start;
} ServoSpeed;

void calHandleDataServo(void *parameters);

void writeServoAngle(void *parameters);


#endif
