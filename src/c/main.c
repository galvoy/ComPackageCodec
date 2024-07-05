#include <stdio.h>

#include "ComPackageCodec.h"

int main(void) {
    char input[256];
    uint8_t outputBuffer[512]; // 假设编码后的数据不会超过512字节
    uint8_t decodedContent[512];
    uint8_t version = 1;
    uint8_t outputVersion;
    size_t contentLength;
    size_t decodedLength;

    while (true) {
        printf("Input Content: ");
        if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n') {
            continue;
        }

        // 去掉换行符
        input[strcspn(input, "\n")] = '\0';
        contentLength = strlen(input);

        size_t encodedLength = EncodeConst253(version, (const uint8_t *) input, contentLength, outputBuffer);
        printf("Encoded: <");
        for (size_t i = 0; i < encodedLength; i++) {
            printf("%02X", outputBuffer[i]);
        }
        printf(">\n");

        // 提取编码后的主体内容
        uint8_t *body = outputBuffer + 1;
        size_t bodyLength = encodedLength - 2;

        bool success = DecodeConst253(body, bodyLength, &outputVersion, decodedContent, &decodedLength);
        if (success) {
            printf("Decoded: <");
            for (size_t i = 0; i < decodedLength; i++) {
                putchar(decodedContent[i]);
            }
            printf(">\n");
        } else {
            printf("Failed to decode the content.\n");
        }
    }

    return 0;
}
