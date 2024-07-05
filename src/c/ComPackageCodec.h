#ifndef C_COMPACKAGECODEC_H
#define C_COMPACKAGECODEC_H

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "stdbool.h"

#include "ComPackageCRC.h"

size_t EncodeConst253(uint8_t version, const uint8_t *content, size_t contentLength, uint8_t *outputBuffer);

bool DecodeConst253(const uint8_t *pkgBody, size_t pkgBodyLength,
                    uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength);

size_t EncodeBase64(uint8_t version, const uint8_t *content, size_t contentLength, uint8_t *outputBuffer);

bool DecodeBase64(const uint8_t *pkgBody, size_t pkgBodyLength,
                  uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength);

#endif
