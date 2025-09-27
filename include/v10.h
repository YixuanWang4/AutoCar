//The header of our file, to let our project pass the compilation
//Variables and functions will be declare here.

#ifndef _c10
#define _c10

#include <arduino.h>

#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include <PS2X_lib.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

//varaible declaration

extern QGPMaker_MotorShield Shield;

extern QGPMaker_Servo * servoL;
extern QGPMaker_Servo * servoR;

extern QGPMaker_DCMotor * motorFL;
extern QGPMaker_DCMotor * motorBL;
extern QGPMaker_DCMotor * motorFR;
extern QGPMaker_DCMotor * motorBR;

extern PS2X ps2x;

extern QueueHandle_t motorSpeedQueue;
extern SemaphoreHandle_t readHandleMutex;

//function declaration

bool initShield(void);
bool pipeInit(void);



#endif