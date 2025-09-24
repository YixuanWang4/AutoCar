#include <Arduino.h>
#include <Adafruit_MS_PWMServoDriver.h>
#include <QGPMaker_MotorShield.h>

//Test cpp for running motor on Esp32 Board

QGPMaker_MotorShield Shield;

QGPMaker_Servo * servo1 = Shield.getServo(0);

void setup(){
    Serial.begin(9600);
    Shield.begin(50);
    Serial.println("Step1 ready!");
};

void loop(){
    uint8_t degree = 0;

    servo1->writeServo(0);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(45);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(90);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(135);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    Serial.println("One loop complete!");
}

