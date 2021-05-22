#include "hc32f00x_rstctl.h"
#include "hc32f005.h"

void rstctl_deinit() { M0P_RESET->RESET_FLAG = 0x00000000; }

uint32_t rstctl_get_flags() { return M0P_RESET->RESET_FLAG; }

void rstctl_peri_reset(enum PERI_RST peri) {
    M0P_RESET->PREI_RESET &= ~(1 << peri);
    for (uint32_t i = 0; i < RESET_CYCLE_NUM; i++) {
        __asm volatile("nop");
    }
    M0P_RESET->PREI_RESET |= (1 << peri);
}
