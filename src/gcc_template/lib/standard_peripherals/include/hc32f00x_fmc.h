#if !defined(__HC32F00X_FMC_H__)
#define __HC32F00X_FMC_H__

#include <stdbool.h>
#include <stdint.h>

enum FMC_MODE {
    FMC_MODE_READ = 0,
    FMC_MODE_PROGRAM,
    FMC_MODE_SECTOR_ERASE,
    FMC_MODE_CHIP_ERASE,
};

enum FMC_PROTECT_INT {
    FMC_PROTECT_INT_NONE = 0x0,
    FMC_PROTECT_INT_PC = 0x1,
    FMC_PROTECT_INT_WP = 0x2,
    FMC_PROTECT_INT_ALL = 0x3,
};

void fmc_deinit();
void fmc_timing_config();
void fmc_sector_we(uint32_t);
bool fmc_busy();
void fmc_mode_set(enum FMC_MODE);
void fmc_protect_int_config(enum FMC_PROTECT_INT);
enum FMC_PROTECT_INT fmc_protect_int_flag_get();
void fmc_protect_int_flag_clear(enum FMC_PROTECT_INT);

#endif // __HC32F00X_FMC_H__
