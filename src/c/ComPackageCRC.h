#ifndef C_COMPACKAGECRC_H
#define C_COMPACKAGECRC_H

#include "stdint.h"

void ComPackageCRC_Init();
void ComPackageCRC_Gen(const uint8_t *data, int dataLength, uint8_t *output);

#endif
