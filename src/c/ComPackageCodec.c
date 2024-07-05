
#include "ComPackageCodec.h"

static size_t EscapeConst253(const uint8_t *data, size_t dataLength, uint8_t *outputDataBuffer) {
    size_t convertedIndex = 0;

    if (data == NULL) return 0;

    for (size_t index = 0; index < dataLength; ++index) {
        uint8_t value = data[index];
        if (value == 253) // 253 => 253 0
        {
            outputDataBuffer[convertedIndex++] = 253;
            outputDataBuffer[convertedIndex++] = 0;
        } else if (value == 254) // 254 => 253 1
        {
            outputDataBuffer[convertedIndex++] = 253;
            outputDataBuffer[convertedIndex++] = 1;
        } else if (value == 255) // 255 => 253 2
        {
            outputDataBuffer[convertedIndex++] = 253;
            outputDataBuffer[convertedIndex++] = 2;
        } else // 无需转义
        {
            outputDataBuffer[convertedIndex++] = value;
        }
    }

    return convertedIndex;
}

static size_t UnescapeConst253(const uint8_t *data, size_t dataLength, uint8_t *outputDataBuffer) {
    size_t convertedIndex = 0;

    size_t maxIndex = dataLength - 1;

    if (outputDataBuffer == NULL) return 0;

    for (size_t index = 0; index < dataLength; ++index) {
        uint8_t value = data[index];
        if (value != 253) // 非转义符
        {
            outputDataBuffer[convertedIndex++] = value;
            continue;
        }

        // 转义符，进入反转义
        if (index + 1 > maxIndex) // 检查越界
        {
            break;
        }

        uint8_t valueNext = data[index + 1];
        index += 1; // 跳过下一个字节

        if (valueNext == 0) // 253 0 => 253
        {
            outputDataBuffer[convertedIndex++] = 253;
        } else if (valueNext == 1) // 253 1 => 254
        {
            outputDataBuffer[convertedIndex++] = 254;
        } else if (valueNext == 2) // 253 2 => 255
        {
            outputDataBuffer[convertedIndex++] = 255;
        } else // 反转义缺省值：253 X => X
        {
            outputDataBuffer[convertedIndex++] = valueNext;
        }
    }

    return convertedIndex;
}

static size_t EncodeGenBodyComputeLength(size_t contentLength) {
    size_t bodyOutputLength = 1 + 2 + contentLength + 2; // version + length + content + CRC
    return bodyOutputLength;
}

static size_t EncodeGenBody(uint8_t version, const uint8_t *content, size_t contentLength, uint8_t *bodyOutputBuffer) {
    size_t bodyOutputLength = EncodeGenBodyComputeLength(contentLength); // version + length + content + CRC

    if (bodyOutputBuffer == NULL) return 0;

    bodyOutputBuffer[0] = version; // version

    // content length
    uint16_t length = (uint16_t) contentLength;
    bodyOutputBuffer[1] = (uint8_t) (length >> 8);
    bodyOutputBuffer[2] = (uint8_t) (length & 0xFF);

    // content
    memcpy(&bodyOutputBuffer[3], content, contentLength);

    // crc
    uint8_t crc[2];
    ComPackageCRC_Gen(content, (int) contentLength, crc);
    bodyOutputBuffer[3 + contentLength] = crc[0];
    bodyOutputBuffer[4 + contentLength] = crc[1];

    return bodyOutputLength;
}

size_t EncodeConst253(uint8_t version, const uint8_t *content, size_t contentLength, uint8_t *outputBuffer) {
    size_t genBodyLength = EncodeGenBodyComputeLength(contentLength);
    uint8_t *bufferGenBody = malloc(genBodyLength);
    EncodeGenBody(version, content, contentLength, bufferGenBody);

    size_t encodedBodyLength = EscapeConst253(bufferGenBody, genBodyLength, outputBuffer + 1);
    free(bufferGenBody);

    size_t outputLength = encodedBodyLength + 2;

    outputBuffer[0] = 255; // package start
    outputBuffer[outputLength - 1] = 254; // package end

    return outputLength;
}

static bool DecodeProcessBody(uint8_t *body, size_t bodyLength,
                              uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength) {
    if (!body || bodyLength <= 5) return false;

    size_t maxIndex = bodyLength - 1;
    *outputVersion = body[0];
    *outputContentLength = (body[1] << 8) | body[2];
    size_t crcStartIndex = 3 + *outputContentLength;
    if (crcStartIndex + 1 > maxIndex) return false;

    uint8_t crc[2];
    ComPackageCRC_Gen(body + 3, (int) *outputContentLength, crc);
    bool crcCorrect = body[crcStartIndex] == crc[0] && body[crcStartIndex + 1] == crc[1];

    if (!crcCorrect) {
        return false;
    }

    memcpy(outputContentBuffer, body + 3, *outputContentLength);

    return true;
}

bool DecodeConst253(const uint8_t *pkgBody, size_t pkgBodyLength,
                    uint8_t *outputVersion, uint8_t *outputContentBuffer, size_t *outputContentLength) {
    uint8_t *buffer = malloc(pkgBodyLength * 2);
    size_t unescapeLength = UnescapeConst253(pkgBody, pkgBodyLength, buffer);
    bool decodeResult = DecodeProcessBody(buffer, unescapeLength, outputVersion, outputContentBuffer,
                                          outputContentLength);
    free(buffer);
    return decodeResult;
}
