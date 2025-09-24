//The header of our file, to let our project pass the compilation
//Variables and functions will be declare here.

#ifndef _c10
#define _c10


#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include <Adafruit_MS_PWMServoDriver.h> //The upper library relies on this library, so we need to include it

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

//varaible declaration

QGPMaker_MotorShield Shield;

QGPMaker_Servo * servoL = Shield.getServo(0);
QGPMaker_Servo * servoR = Shield.getServo(1);

QGPMaker_DCMotor * motorFL = Shield.getMotor(1);
QGPMaker_DCMotor * motorBL = Shield.getMotor(2);
QGPMaker_DCMotor * motorFR = Shield.getMotor(3);
QGPMaker_DCMotor * motorBR = Shield.getMotor(4);

//function declaration

bool initShield(void);
bool pipeInit(void);






#endif