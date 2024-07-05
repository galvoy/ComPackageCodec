#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "RingBuffer.h"

// 初始化环形缓冲区
void RingBufferInit(RingBuffer *rb, size_t size) {
    rb->buffer = malloc(size);
    if (rb->buffer == NULL) {
        // 处理分配错误
        perror("分配环形缓冲区失败");
        exit(EXIT_FAILURE);
    }
    rb->max = size;
    rb->head = 0;
    rb->tail = 0;
    rb->full = false;
    if (pthread_mutex_init(&rb->lock, NULL) != 0) {
        // 处理互斥量创建失败
        perror("创建互斥量失败");
        free(rb->buffer);
        exit(EXIT_FAILURE);
    }
}

// 重置环形缓冲区
void RingBufferReset(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->full = false;
}

// 释放环形缓冲区
void RingBufferFree(RingBuffer *rb) {
    free(rb->buffer);
    pthread_mutex_destroy(&rb->lock); // 销毁互斥量
}

// 判断环形缓冲区是否为空
bool RingBufferIsEmpty(RingBuffer *rb) {
    return (rb->head == rb->tail) && !rb->full;
}

// 判断环形缓冲区是否已满
bool RingBufferIsFull(RingBuffer *rb) {
    return rb->full;
}

// 获取环形缓冲区中的数据量
size_t RingBufferCurrentDataSize(RingBuffer *rb) {
    if (rb->full) {
        return rb->max;
    }
    if (rb->head >= rb->tail) {
        return rb->head - rb->tail;
    }
    return rb->max + rb->head - rb->tail;
}

// 向环形缓冲区添加数据
bool RingBufferAdd(RingBuffer *rb, uint8_t data) {
    rb->buffer[rb->head] = data;

    if (rb->full) {
        // 如果缓冲区已满，头指针也需要移动
        rb->tail = (rb->tail + 1) % rb->max;
        return false;
    }

    rb->head = (rb->head + 1) % rb->max;
    rb->full = (rb->head == rb->tail);

    return true;
}

bool RingBufferAddSecurity(RingBuffer *rb, uint8_t data) {
    bool result = false;
    // 获取互斥量，如果当前被其他任务占用则等待
    if (pthread_mutex_lock(&rb->lock) == 0) {
        // 成功获取锁
        result = RingBufferAdd(rb, data);
        // 释放互斥量
        pthread_mutex_unlock(&rb->lock);
    }
    return result;
}

bool RingBufferAddRangeSecurity(RingBuffer *rb, uint8_t *data, uint32_t length) {
    bool result = true;
    // 获取互斥量，如果当前被其他任务占用则等待
    if (pthread_mutex_lock(&rb->lock) == 0) {
        // 成功获取锁
        if (length > 0) {
            for (int index = 0; index < length; index++) {
                if (!RingBufferAdd(rb, data[index])) {
                    result = false;
                    break;
                }
            }
        }
        // 释放互斥量
        pthread_mutex_unlock(&rb->lock);
    }
    return result;
}

// 从环形缓冲区取数据
bool RingBufferGet(RingBuffer *rb, uint8_t *data) {
    if (RingBufferIsEmpty(rb)) {
        // 缓冲区为空，无法取数据
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->max;
    rb->full = false;
    return true;
}

bool RingBufferGetSecurity(RingBuffer *rb, uint8_t *data) {
    // 获取互斥量，如果当前被其他任务占用则等待
    bool result = false;
    if (pthread_mutex_lock(&rb->lock) == 0) {
        // 成功获取锁
        if (RingBufferIsEmpty(rb)) {
            // 缓冲区为空，无法取数据
            result = false;
        } else {
            *data = rb->buffer[rb->tail];
            rb->tail = (rb->tail + 1) % rb->max;
            rb->full = false;
            result = true;
        }
        // 释放互斥量
        pthread_mutex_unlock(&rb->lock);
    }
    return result;
}

bool RingBufferGetBytesSecurity(RingBuffer *rb, uint8_t *data, size_t *actualLength, size_t maxLength) {
    bool result = false;
    // 获取互斥量，如果当前被其他任务占用则等待
    if (pthread_mutex_lock(&rb->lock) == 0) {
        // 成功获取锁
        *actualLength = 0;
        for (int index = 0; index < maxLength; index++) {
            if (RingBufferGet(rb, data + index)) {
                *actualLength += 1;
            } else {
                break;
            }
        }
        result = *actualLength != 0;
        // 释放互斥量
        pthread_mutex_unlock(&rb->lock);
    }
    return result;
}

