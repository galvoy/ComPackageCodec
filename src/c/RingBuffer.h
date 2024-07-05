
#ifndef C_RINGBUFFER_H
#define C_RINGBUFFER_H


#include <stdint.h>
#include <stdlib.h>
#include "stdbool.h"
#include <pthread.h>

typedef struct {
    uint8_t *buffer; // 实际数据缓冲区
    size_t head;     // 头索引
    size_t tail;     // 尾索引
    size_t max;      // 缓冲区大小
    bool full;       // 环形缓冲区是否已满
    pthread_mutex_t lock;
} RingBuffer;

void RingBufferInit(RingBuffer *rb, size_t size);

void RingBufferReset(RingBuffer *rb);

void RingBufferFree(RingBuffer *rb);

bool RingBufferIsEmpty(RingBuffer *rb);

bool RingBufferIsFull(RingBuffer *rb);

size_t RingBufferCurrentDataSize(RingBuffer *rb);

bool RingBufferAddSecurity(RingBuffer *rb, uint8_t data);

bool RingBufferAddRangeSecurity(RingBuffer *rb, uint8_t *data, uint32_t length);

bool RingBufferGetSecurity(RingBuffer *rb, uint8_t *data);

bool RingBufferGetBytesSecurity(RingBuffer *rb, uint8_t *data, size_t *actualLength, size_t maxLength);


#endif
