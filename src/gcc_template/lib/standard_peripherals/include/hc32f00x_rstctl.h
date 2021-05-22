#if !defined(__HC32F00X_RSTCTL_H__)
#define __HC32F00X_RSTCTL_H__

#include <stdbool.h>
#include <stdint.h>

#define RESET_CYCLE_NUM 16

enum RST_FLAG {
    RST_FLAG_POR5V = 0x1,
    RST_FLAG_POR15V = 0x2,
    RST_FLAG_LVD = 0x4,
    RST_FLAG_WDT = 0x8,
    RST_FLAG_PCA = 0x10,
    RST_FLAG_LOCKUP = 0x20,
    RST_FLAG_SYSREQ = 0x40,
    RST_FLAG_RSTB = 0x80,
};

enum PERI_RST {
    PERI_RST_UART0 = 0,
    PERI_RST_UART1 = 1,
    PERI_RST_I2C = 4,
    PERI_RST_SPI = 6,
    PERI_RST_BT = 8,
    PERI_RST_ADT = 10,
    PERI_RST_PCA = 14,
    PERI_RST_ADC = 16,
    PERI_RST_VC_LVD = 17,
    PERI_RST_TRIM = 21,
    PERI_RST_TICK = 24,
    PERI_RST_CRC = 26,
    PERI_RST_GPIO = 28,
};

void rstctl_deinit();
uint32_t rstctl_get_flags();
void rstctl_peri_reset(enum PERI_RST);

#endif // __HC32F00X_RSTCTL_H__
