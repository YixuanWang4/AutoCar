#include <Arduino.h>
#include <Adafruit_MS_PWMServoDriver.h>
#include <QGPMaker_MotorShield.h>
#include <wheelcal.h>

QGPMaker_DCMotor * motor1 = Shield.getMotor(1);
QGPMaker_DCMotor * motor2 = Shield.getMotor(2);
QGPMaker_DCMotor * motor3 = Shield.getMotor(3);
QGPMaker_DCMotor * motor4 = Shield.getMotor(4);

void setup(){

    Shield.begin(50);

    motor1->setSpeed(128);
    motor1->run(FORWARD);
    motor2->setSpeed(128);
    motor2->run(FORWARD);
    motor3->setSpeed(128);
    motor3->run(FORWARD);
    motor4->setSpeed(128);
    motor4->run(FORWARD);

}

void loop(){
    
}