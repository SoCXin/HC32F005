#include "hc32f00x_crc16.h"
#include "HC32F005.h"

void crc16_deinit() { M0P_CRC->RESULT = 0x0000ffff; }

void crc16_put_word(uint32_t data) { M0P_CRC->DATA = data; }

uint16_t crc16_get_result() { return M0P_CRC->RESULT_f.RESULT; }
