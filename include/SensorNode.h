#ifndef SENSORNODE_H
#define SENSORNODE_H

#define STEPS_PER_REVOLUTION 200
#define REVOLUTIONS 4//18

#include <Stepper.h>
#include "TOF400F.h"
#include "CRC_16_MODBUS.h"
#include "MoistureSensor.h"

class SensorNode {
    private:
        TOF400F sensor1;
        TOF400F sensor2;
        TOF400F sensor3;
        TOF400F sensor4;
        HardwareSerial* uart;
        unsigned long baudRate;
        CRC_16_MODBUS crcCheck;
        Stepper myStepper;
        MoistureSensor moistureSensor;

    public:
        SensorNode(int tx1, int rx1, int tx2, int rx2, int tx3, int rx3, int tx4, int rx4, int in1, int in2, int in3, int in4, unsigned long baudRate, HardwareSerial* uart);
        void init();
        unsigned long readSensorData(int index);
        String readAllSensorData();
        void listenForRequests();
        void moveUp();
        void moveDown();
};

#endif


