#include "SensorNode.h"

SensorNode::SensorNode(int tx1, int rx1, int tx2, int rx2, int tx3, int rx3, int tx4, int rx4, int in1, int in2, int in3, int in4, unsigned long baudRateArg, HardwareSerial* uartPort) 
: sensor1(tx1, rx1), sensor2(tx2, rx2), sensor3(tx3, rx3), sensor4(tx4, rx4), uart(uartPort), myStepper(STEPS_PER_REVOLUTION, in1, in2, in3, in4), moistureSensor(23, 19)
{
    baudRate = baudRateArg;
}
void SensorNode::init(){
    uart->begin(baudRate);
    sensor1.init();
    sensor2.init();
    sensor3.init();
    sensor4.init();
    moistureSensor.init();
    myStepper.setSpeed(170);
}

unsigned long SensorNode::readSensorData(int index) {
    switch (index)
    {
    case 1:
        return sensor1.readSensorData();
    case 2:
        return sensor2.readSensorData();
    case 3:
        return sensor3.readSensorData();
    case 4:
        return sensor4.readSensorData();
    case 5:
        return moistureSensor.readSensorData();

    default:
        return -9;
    }
}

String SensorNode::readAllSensorData() {
    String allData =  "1: " + String(sensor1.readSensorData()) + " 2: " + String(sensor2.readSensorData()) + " 3: " + String(sensor3.readSensorData()) + "  4: " + String(sensor4.readSensorData());
    return allData;
}

void SensorNode::moveUp() {
    Serial.println("stepper moving up");
    myStepper.step(REVOLUTIONS*STEPS_PER_REVOLUTION);
    uart->println("moving up");
}

void SensorNode::moveDown() {
    Serial.println("stepper moving down");
    myStepper.step(-REVOLUTIONS*STEPS_PER_REVOLUTION);
    uart->println("moving down");
}

void SensorNode::listenForRequests() {
    const int bufferSize = 16;
    char requestBuffer[bufferSize];  // Pre-allocated buffer
    int index = 0;

    if (uart->available()) {
        // Read request byte-by-byte until newline or buffer is full
        while (uart->available() && index < bufferSize - 1) {
            char c = uart->read();
            if (c == '\n') {
                break;  // End of request
            }
            requestBuffer[index++] = c;
        }
        requestBuffer[index] = '\0';
        if (strncmp(requestBuffer, "up", 2) == 0) {
            Serial.println("in up");
            moveUp();
        } else if (strncmp(requestBuffer, "down", 4) == 0) {
            Serial.println("in down");
            moveDown();
        } else if (strncmp(requestBuffer, "get_", 4) == 0 && isDigit(requestBuffer[4]) && requestBuffer[4] < '5') {

            int sensorIndex = requestBuffer[4] - '0';  // Get sensor index from request
            Serial.println(sensorIndex);
            String value = String(readSensorData(sensorIndex));
            // uint16_t crc = crcCheck.calculateCRC((uint8_t*)value.c_str(), value.length());
            uart->println(value);  // Read specific sensor data
            Serial.print("dis: ");
            // uart->println(crc);
            Serial.println(value);
            // Serial.println(crc);
        } else if (strncmp(requestBuffer, "get_", 4) == 0 && requestBuffer[4] == '5') {
            String value = String(readSensorData(5));
            Serial.print("hum: ");
            uart->println(value);
            Serial.println(value);
        } else if (strcmp(requestBuffer, "AT") == 0) {
            uart->println("OK");  // Respond to AT command
        } else {
            uart->println("request error");  // Unknown request
        }
    }
}


