#include "hc32f00x_basetimer.h"
#include "hc32f005.h"

void basetimer_deinit(M0P_BT_TypeDef *timer) {
    timer->ARR = 0;
    timer->CNT = 0;
    timer->CNT32 = 0;
    timer->CR = 0x00000008;
    timer->IFR = 0x00000000;
    timer->ICLR = 0x00000000;
}

void basetimer_config(M0P_BT_TypeDef *timer, bool ie,
                      enum BASETIMER_GATE_P gate_p, bool gate,
                      enum BASETIMER_DIV ck_div, bool tog_en,
                      enum BASETIMER_FUNC func, enum BASETIMER_MODE mode) {
    timer->CR_f.IE = ie;
    timer->CR_f.GATE_P = gate_p;
    timer->CR_f.GATE = gate;
    timer->CR_f.PRS = ck_div;
    timer->CR_f.TOG_EN = tog_en;
    timer->CR_f.CT = func;
    timer->CR_f.MD = mode;
}

void basetimer_set_auto_reload_value(M0P_BT_TypeDef *timer, uint16_t val) {
    timer->ARR = val;
}

void basetimer_set_count_value_16(M0P_BT_TypeDef *timer, uint16_t val) {
    timer->CNT = val;
}

uint16_t basetimer_get_count_value_16(M0P_BT_TypeDef *timer) {
    return timer->CNT;
}

void basetimer_set_count_value_32(M0P_BT_TypeDef *timer, uint32_t val) {
    timer->CNT32 = val;
}

uint32_t basetimer_get_count_value_32(M0P_BT_TypeDef *timer) {
    return timer->CNT32;
}

bool basetimer_check_int_flag(M0P_BT_TypeDef *timer) { return timer->IFR_f.TF; }

void basetimer_clear_int_flag(M0P_BT_TypeDef *timer) { timer->ICLR_f.TFC = 0; }
