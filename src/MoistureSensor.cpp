#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int tx, int rx) {
    uart = new SoftwareSerial(tx, rx);
}

void MoistureSensor::init(){
    uart->begin(MOISTURE_SENSOR_BAUDRATE);
}
void printHexByte(byte b){
  Serial.print((b >> 4) & 0xF, HEX);
  Serial.print(b & 0xF, HEX);
  Serial.print(' ');
}

uint16_t MoistureSensor::readSensorData() {
    uint32_t startTime = 0;
    uint8_t byteCount = 0;

    uart->write(all, sizeof(all));
    uart->flush();

    startTime = millis();
    while (millis() - startTime <= MOISTURE_SENSOR_TIMEOUT) {
        if (uart->available() && byteCount < sizeof(values)) {
            values[byteCount++] = uart->read();
            // printHexByte(values[byteCount - 1]);
        }
    }

  // Parse and calculate the Moisture value
    int Moisture_Int = int(values[3] << 8 | values[4]);
    // float Moisture_Percent = Moisture_Int / 10.0;

    return Moisture_Int;    

}