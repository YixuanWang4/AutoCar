//The header of our file, to let our project pass the compilation
//Variables and functions will be declare here.

#ifndef _c10
#define _c10

#define USE_ESP32
//#define USE_ESP32_S3

//#define USE_IIC_STANDARD
#define USE_IIC_FAST

#include <Arduino.h> //To enable the basic Arduino function

#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include <Ultrasonic.h>
#include <PS2X_lib.h>

#include <wheelcal.h>
#include <servocal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

//varaible declaration

QueueHandle_t motorSpeedQueue;
QueueHandle_t servoSpeedQueue;
SemaphoreHandle_t readHandleMutex;

TickType_t readHandleDelay = 8;//The rate of reading handle 8
TickType_t calMotorDelay = 15;//The rate of calculating data 15
TickType_t calServoDelay = 20; //30
TickType_t writeDelay = 4;//The rate of writing data to motor 5
TickType_t writeServoDelay = 10; //15

TaskHandle_t servoHandle;

int suOffset = 12;
int sdOffset = -4;

float maxSpeed = 500;//The maxspeed of minicar, default to 1000 mm/s
float angle = 0.7153829260635;
float length = 0.136452372643;
float radius = 0.031;//The radius of our wheel

extern QGPMaker_MotorShield Shield;

extern QGPMaker_DCMotor * motorFL;
extern QGPMaker_DCMotor * motorBL;
extern QGPMaker_DCMotor * motorFR;
extern QGPMaker_DCMotor * motorBR;

extern QGPMaker_Servo * servoD;
extern QGPMaker_Servo * servoU;
extern QGPMaker_Servo * servoR;
extern QGPMaker_Servo * servoF;

extern PS2X ps2x;

extern QueueHandle_t motorSpeedQueue;
extern QueueHandle_t servoSpeedQueue;
extern SemaphoreHandle_t readHandleMutex;

//function declaration

bool initShield(void);
bool pipeInit(void);
bool servoInit(void);

#endif