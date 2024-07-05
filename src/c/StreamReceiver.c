
#include "StreamReceiver.h"

void StreamReceiver_New(StreamReceiver *instance, size_t bufferSize) {
    RingBufferInit(instance->buffer, bufferSize);
}

bool StreamReceiver_AddByte(StreamReceiver *instance, uint8_t data) {
    RingBufferAddSecurity(instance->buffer, data);
    if (data != 254) {
        return false;
    } else {
        return true;
    }
}

bool StreamReceiver_ReadPackage(StreamReceiver *instance,
                                uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength) {
    size_t dataExistLength = RingBufferCurrentDataSize(instance->buffer);
    uint8_t *pkgBuffer = malloc(dataExistLength);
    size_t dataLength;
    RingBufferGetBytesSecurity(instance->buffer, pkgBuffer, &dataLength, dataExistLength);
    size_t maxIndex = dataLength - 1;
    int pkgHeadIndex = -1;
    for (int index = 0; index <= maxIndex - 1; index++) {
        if (pkgBuffer[index] == 255 && pkgBuffer[index + 1] != 255) {
            pkgHeadIndex = index;
            break;
        }
    }
    int bodyStartIndex = pkgHeadIndex + 1;
    bool result = DecodeConst253(pkgBuffer + bodyStartIndex, maxIndex - bodyStartIndex,
                               outputVersion, outputContentBuffer, outputContentLength);
    free(pkgBuffer);
    return result;
}

void StreamReceiver_Dispose(StreamReceiver *instance) {
    RingBufferFree(instance->buffer);
}
