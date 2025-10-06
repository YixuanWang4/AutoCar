#include <Arduino.h>
#include <Adafruit_MS_PWMServoDriver.h>
#include <QGPMaker_MotorShield.h>

#include <servocal.h>

//Test cpp for running servos on Esp32 Board

QGPMaker_Servo * servo1 = Shield.getServo(0);
QGPMaker_Servo * servo2 = Shield.getServo(1);

void setup(){
    Serial.begin(9600);
    Shield.begin(50);
    Serial.println("Step1 ready!");
};

void loop(){
    uint8_t degree = 0;

    servo1->writeServo(0);
    servo2->writeServo(0);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(30);
    servo2->writeServo(30);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(60);
    servo2->writeServo(60);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    servo1->writeServo(90);
    servo2->writeServo(90);
    degree = servo1->readDegrees();
    Serial.println(degree);
    delay(1000);

    Serial.println("One loop complete!");
}

