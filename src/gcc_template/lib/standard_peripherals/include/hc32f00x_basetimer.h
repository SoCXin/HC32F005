#if !defined(__HC32F00X_BASETIMER_H__)
#define __HC32F00X_BASETIMER_H__

#include "hc32f005.h"
#include <stdbool.h>
#include <stdint.h>

enum BASETIMER_MODE {
    BASETIMER_MODE_32_NO_AUTORELOAD = 0,
    BASETIMER_MODE_16_AUTORELOAD,
};

enum BASETIMER_GATE_P {
    BASETIMER_GATE_POS = 0,
    BASETIMER_GATE_NEG,
};

enum BASETIMER_DIV {
    BASETIMER_DIV_1 = 0,
    BASETIMER_DIV_2,
    BASETIMER_DIV_4,
    BASETIMER_DIV_8,
    BASETIMER_DIV_16,
    BASETIMER_DIV_32,
    BASETIMER_DIV_64,
    BASETIMER_DIV_256,
};

enum BASETIMER_FUNC {
    BASETIMER_FUNC_TIMER = 0,
    BASETIMER_FUNC_COUNTER,
};

void basetimer_deinit(M0P_BT_TypeDef *);
void basetimer_config(M0P_BT_TypeDef *, bool ie, enum BASETIMER_GATE_P,
                      bool gate, enum BASETIMER_DIV, bool tog_en,
                      enum BASETIMER_FUNC, enum BASETIMER_MODE);
void basetimer_set_auto_reload_value(M0P_BT_TypeDef *, uint16_t val);
void basetimer_set_count_value_16(M0P_BT_TypeDef *, uint16_t val);
uint16_t basetimer_get_count_value_16(M0P_BT_TypeDef *);
void basetimer_set_count_value_32(M0P_BT_TypeDef *, uint32_t val);
uint32_t basetimer_get_count_value_32(M0P_BT_TypeDef *);
bool basetimer_check_int_flag(M0P_BT_TypeDef *);
void basetimer_clear_int_flag(M0P_BT_TypeDef *);

#endif // __HC32F00X_BASETIMER_H__
