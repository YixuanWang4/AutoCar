/*cpp file to store calculations regarding wheel speed*/

#include <wheelcal.h>

#include <freertos/FreeRTOS.h> //To enable the freeRTOS system, so our car will respond more quickly
#include <freertos/queue.h>
#include <freertos/semphr.h>

void calHandlePara(void * pvParameters){

}//Receive data from handle and turn it into CarStatus

void calMotorAngSpd(void * pvParameters){
 
};//Receive CarStatus and calculate angular speed

void writeMotorAngSpd(void * pvParameters){

};//Write speed order to the motor