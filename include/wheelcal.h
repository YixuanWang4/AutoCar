//The header of wheelcal.cpp

#ifndef _wheelcal
#define _wheelcal

//varaibles definition
typedef struct {
    float vx;
    float vy;
    float w;
    float angle;
    float length;
    float radius;
} CarStatus;


//function definition
void calMotorAngSpd(void *pvParameters);

void writeMotorAngSpd(void *pvParameters);

void calHandlePara(void *pvParameters);


#endif