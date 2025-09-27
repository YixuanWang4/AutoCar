//The header of wheelcal.cpp

#ifndef _wheelcal
#define _wheelcal

#include <arduino.h>
#include <QGPMaker_MotorShield.h> //To communicate with our Motorshield
#include <PS2X_lib.h>

//varaibles definition
typedef struct {
    bool padUp, padDown, padLeft, padRight, 
         l1, l2, r1, r2,
         green, red, blue, pink,
         select, start;
    byte lx, ly, rx, ry;

} HandleData;

typedef struct {
    float vx, vy, w;

} CarStatus;

typedef struct {
    float wFL, wFR, wBL, wBR;
} MotorSpeed;

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

//properties of minicar

extern TickType_t readHandleDelay;//The rate of reading handle
extern TickType_t calHandleDelay;//The rate of calculating data
extern TickType_t writeDelay;//The rate of writing data to motor
extern float maxSpeed;//The maxspeed of minicar, default to 1000 mm/s
extern float angle;//Currently not designed, default to arctan(0.75)
extern float length;//Currently not designed, default to 0.25
extern float radius;//The radius of our wheel

//function definition
void writeMotorAngSpd(void *pvParameters);

void calHandlePara(void *pvParameters);

void getHandleData(void *pvParameters);

#endif