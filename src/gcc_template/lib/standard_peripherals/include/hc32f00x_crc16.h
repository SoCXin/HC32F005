#if !defined(__HC32F00X_CRC16_H__)
#define __HC32F00X_CRC16_H__

#include <stdint.h>

void crc16_deinit();
void crc16_put_word(uint32_t);
uint16_t crc16_get_result();

#endif // __HC32F00X_CRC16_H__
