//The header of our file, to let our project pass the compilation
//Variables and functions will be declare here.

#ifndef _c10
#define _c10
#endif

#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include <Adafruit_MS_PWMServoDriver.h> //The upper library relies on this library, so we need to include it

//varaible declaration

QGPMaker_MotorShield Shield;

QGPMaker_Servo * servoLF = Shield.getServo(0);
QGPMaker_Servo * servoLB = Shield.getServo(1);

//function declaration

bool initShield(void);