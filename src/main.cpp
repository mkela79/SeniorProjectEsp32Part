#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SensorNode.h"
#include "MoistureSensor.h"

#define RX_PIN_1 18
#define TX_PIN_1 -1
#define RX_PIN_2 25
#define TX_PIN_2 -1
#define RX_PIN_3 32
#define TX_PIN_3 -1
#define RX_PIN_4 4
#define TX_PIN_4 -1
#define ESP_COM_BAUDRATE 115200

#define STEPPER_IN1 13
#define STEPPER_IN2 14
#define STEPPER_IN3 27
#define STEPPER_IN4 33

SensorNode sensorNode(RX_PIN_1, TX_PIN_1, RX_PIN_2, TX_PIN_2, RX_PIN_3, TX_PIN_3, RX_PIN_4, TX_PIN_4, STEPPER_IN1, STEPPER_IN3, STEPPER_IN2, STEPPER_IN4, ESP_COM_BAUDRATE, &Serial2); // RX, TX


void setup() {
  Serial.begin(115200);
  sensorNode.init();
  
  Serial.println("Sensor Test 2"); 
}

void loop() {
    sensorNode.listenForRequests();
}