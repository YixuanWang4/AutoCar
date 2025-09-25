/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

void getHandleData(void){

};//Get data form our handle

void calHandlePara(void){

};//Receive data from handle and turn it into CarStatus

void calMotorAngSpd(void){
 
};//Receive CarStatus and calculate angular speed

void writeMotorAngSpd(void){

};//Write speed order to the motor