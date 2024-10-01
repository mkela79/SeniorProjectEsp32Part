#include "TOF400F.h"
int timeout = 100;

TOF400F::TOF400F(int tx, int rx) {
    softwareSerial = new SoftwareSerial(tx, rx);
}  

void TOF400F::init() {
    softwareSerial->begin(115200);
}

uint16_t TOF400F::calculateCRC(uint8_t* data, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for(uint8_t pos = 0; pos < length; pos++) {
        crc ^= (uint16_t) data[pos];

        for(uint8_t i = 8; i != 0; i--) {
            if((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

bool TOF400F::checkCRC(uint8_t*  data) {
    uint16_t receivedCRC = (data[6] << 8) | data[5];
    uint16_t calculatedCRC = calculateCRC(data, 5);

    return receivedCRC == calculatedCRC;
}

unsigned long TOF400F::readSensorData() {
    unsigned long startTime = millis();
    uint8_t rx_buffer[16];
    uint8_t index = 0;
    bool foundETX = false;
    // Serial.println("pre");
    while (millis() - startTime < timeout) {
        
        if (softwareSerial->available()) {
            uint8_t byte = softwareSerial->read();
            
            if (foundETX) {
                rx_buffer[index++] = byte;

                if(index == 1 && byte != 2) {
                    foundETX = false;
                    index = 0;
                }
                
                if (index >= 5) {
                    uint8_t data[16];

                    data[0] = 0x01;
                    data[1] = 0x03;
                    data[2] = 0x02;
                    data[3] = rx_buffer[1];
                    data[4] = rx_buffer[2];
                    data[5] = rx_buffer[3];
                    data[6] = rx_buffer[4];

                    uint16_t distance = (data[3] << 8) | data[4];
                    
                    if (distance < 4001 ) {  
                        if(checkCRC(data)) {
                            lastValue = distance;
                            return distance;
                        }
                        
                    }
                    return lastValue;
                }
            } else if (byte == 3) { 
                foundETX = true;
                index = 0;
            } else {
                foundETX = false;
                index = 0;
            }
        }
    }
    return lastValue;  // Return last valid value if timeout occurs
}