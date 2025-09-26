#include <Arduino.h>
#include <test2.h>

#include <PS2X_lib.h>

unsigned long lastTime = 0;
PS2X ps2x;

void setup(){
    Serial.begin(9600);
    int error = ps2x.config_gamepad(18,23,5,19, true, true);
    if(error != 0){
        Serial.println("W1 Found");
    };
    bool init = ps2x.read_gamepad(false, 0);
    if(init != 1){
        Serial.println("W2 Found");
    };
};

void loop(){
    ps2x.read_gamepad(false, 0);
    if(ps2x.Button(PSB_PAD_UP)){
        Serial.println("(PSB_PAD_UP)");
    };

    if(ps2x.Button(PSB_L2)){
        Serial.println("(PSB_L2)");
    };

    if(ps2x.Button(PSB_R2)){
        Serial.println("(PSB_R2)");
    };

    if(ps2x.Button(PSB_PAD_DOWN)){
        Serial.println("(PSB_PAD_DOWN)");
    };

    if(ps2x.Button(PSB_PAD_LEFT)){
        Serial.println("(PSB_PAD_LEFT)");
    };

    if(ps2x.Button(PSB_PAD_RIGHT)){
        Serial.println("(PSB_PAD_RIGHT)");
    };

    if(ps2x.Button(PSB_L1)){
        Serial.println("(PSB_L1)");
    };

    if(ps2x.Button(PSB_R1)){
        Serial.println("(PSB_R1)");
    };

    if(ps2x.Analog(PSS_LX) > 130){
        Serial.print("(PSB_LX):");
        Serial.println(ps2x.Analog(PSS_LX));
    };

    if(ps2x.Analog(PSS_LY) > 130){
        Serial.print("(PSB_LY):");
        Serial.println(ps2x.Analog(PSS_LY));
    };
    
    if(ps2x.Analog(PSS_RY) > 130){
        Serial.print("(PSB_RY):");
        Serial.println(ps2x.Analog(PSS_RY));
    };

    if(ps2x.Analog(PSS_RX) > 130){
        Serial.print("(PSB_RX):");
        Serial.println(ps2x.Analog(PSS_RX));
    };

    if(ps2x.Button(PSB_GREEN)){
        Serial.println("(PSB_GREEN)");
    };

    if(ps2x.Button(PSB_RED)){
        Serial.println("(PSB_RED)");
    };

    if(ps2x.Button(PSB_BLUE)){
        Serial.println("(PSB_BLUE)");
    };

    if(ps2x.Button(PSB_PINK)){
        Serial.println("(PSB_PINK)");
    };

    if(ps2x.Button(PSB_SELECT)){
        Serial.println("(PSB_SELECT)");
    };

    if(ps2x.Button(PSB_START)){
        Serial.println("(PSB_START)");
    };
    
    //Serial.print(millis() - lastTime);
    //Serial.println("ms.");
    lastTime = millis();
    delay(10);

};
