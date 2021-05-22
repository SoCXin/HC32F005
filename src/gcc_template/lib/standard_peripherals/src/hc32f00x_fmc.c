#include "hc32f00x_fmc.h"
#include "HC32F005.h"

void fmc_we() {
    M0P_FLASH->BYPASS = 0x5a5a;
    M0P_FLASH->BYPASS = 0xa5a5;
}

void fmc_deinit() {
    fmc_we();
    M0P_FLASH->TNVS = 0x00000020;
    fmc_we();
    M0P_FLASH->TPGS = 0x00000017;
    fmc_we();
    M0P_FLASH->TPROG = 0x0000001b;
    fmc_we();
    M0P_FLASH->TSERASE = 0x00004650;
    fmc_we();
    M0P_FLASH->TMERASE = 0x000222e0;
    fmc_we();
    M0P_FLASH->TPRCV = 0x00000018;
    fmc_we();
    M0P_FLASH->TSRCV = 0x000000f0;
    fmc_we();
    M0P_FLASH->TMRCV = 0x000003e8;
    fmc_we();
    M0P_FLASH->CR = 0x00000000;
    fmc_we();
    M0P_FLASH->IFR = 0x00000000;
    fmc_we();
    M0P_FLASH->ICLR = 0x0000000f;
    fmc_we();
    M0P_FLASH->SLOCK = 0x00000000;
}

extern uint32_t SystemCoreClock;
void fmc_timing_config() {
    uint32_t mul = SystemCoreClock / 4000000;
    if (mul > 1) {
        fmc_we();
        M0P_FLASH->TNVS = 0x00000020 * mul;
        fmc_we();
        M0P_FLASH->TPGS = 0x00000017 * mul;
        fmc_we();
        M0P_FLASH->TPROG = 0x0000001b * mul;
        fmc_we();
        M0P_FLASH->TSERASE = 0x00004650 * mul;
        fmc_we();
        M0P_FLASH->TMERASE = 0x000222e0 * mul;
        fmc_we();
        M0P_FLASH->TPRCV = 0x00000018 * mul;
        fmc_we();
        M0P_FLASH->TSRCV = 0x000000f0 * mul;
        fmc_we();
        M0P_FLASH->TMRCV = 0x000003e8 * mul;
    }
    if (mul > 6) {
        fmc_we();
        M0P_FLASH->CR_f.WAIT = 1;
    }
}

void fmc_sector_we(uint32_t mask) {
    fmc_we();
    M0P_FLASH->SLOCK = mask;
}

bool fmc_busy() { return M0P_FLASH->CR_f.BUSY; }

void fmc_mode_set(enum FMC_MODE mode) {
    fmc_we();
    M0P_FLASH->CR_f.OP = mode;
}

void fmc_protect_int_config(enum FMC_PROTECT_INT cfg) {
    fmc_we();
    M0P_FLASH->CR_f.IE = cfg;
}

enum FMC_PROTECT_INT fmc_protect_int_flag_get() { return M0P_FLASH->IFR; }

void fmc_protect_int_flag_clear(enum FMC_PROTECT_INT bits) {
    fmc_we();
    M0P_FLASH->ICLR = ~bits; // write 0 to clear
}
