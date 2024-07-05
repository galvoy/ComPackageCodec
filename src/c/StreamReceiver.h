#ifndef C_STREAMRECEIVER_H
#define C_STREAMRECEIVER_H

#include "RingBuffer.h"
#include "ComPackageCodec.h"

typedef struct {
    RingBuffer *buffer;
} StreamReceiver;

void StreamReceiver_New(StreamReceiver *instance, size_t bufferSize);

bool StreamReceiver_AddByte(StreamReceiver *instance, uint8_t data);

bool StreamReceiver_ReadPackage(StreamReceiver *instance,
                                uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength);

void StreamReceiver_Dispose(StreamReceiver *instance);

#endif
