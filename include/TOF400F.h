#ifndef TOF400F_H
#define TOF400F_H

#define TOF400F_BAUDRATE 115200

#include <Arduino.h>
#include <SoftwareSerial.h>

class TOF400F {
    private:
        SoftwareSerial* softwareSerial;
        unsigned long lastValue = 50;
    public:
        TOF400F(int tx, int rx);
        void init();
        unsigned long readSensorData();
        unsigned long hexCharToUL(uint8_t c);
        uint8_t reflectByte(uint8_t data);
        uint16_t reflectCRC16(uint16_t crc);
        uint16_t calculateCRC(uint8_t* data, uint8_t length);
        bool checkCRC(uint8_t*  data);
        uint8_t readDistance(uint16_t &distance, uint32_t timeout = 500);
};

#endif
