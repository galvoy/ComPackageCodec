
#include "ComPackageCRC.h"

static const uint16_t crcPolynomial = 0x1021;
static uint16_t crcTable[256];

void ComPackageCRC_Init() {
    uint16_t value;
    uint16_t temp;
    for (uint16_t i = 0; i < 256; ++i) {
        value = 0;
        temp = i;
        for (uint8_t j = 0; j < 8; ++j) {
            if (((value ^ temp) & 0x0001) != 0) {
                value = (uint16_t) ((value >> 1) ^ crcPolynomial);
            } else {
                value >>= 1;
            }
            temp >>= 1;
        }
        crcTable[i] = value;
    }
}

void ComPackageCRC_Gen(const uint8_t *data, int dataLength, uint8_t *output) {
    uint16_t crcValue = 0xffff;

    for (int i = 0; i < dataLength; ++i) {
        uint8_t index = (uint8_t) (((crcValue >> 8) ^ data[i]) & 0xff);
        crcValue = (uint16_t) ((crcValue << 8) ^ crcTable[index]);
    }

    output[0] = (uint8_t) (crcValue & 0xFF);
    output[1] = (uint8_t) ((crcValue >> 8) & 0xFF);
}
