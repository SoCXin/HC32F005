#if !defined(__HC32F00X_SYSCTL_H__)
#define __HC32F00X_SYSCTL_H__

#include <stdbool.h>

enum RCH_TRIM {
    RCH_TRIM_24M = 0x00100c00,
    RCH_TRIM_22POINT12M = 0x00100c02,
    RCH_TRIM_16M = 0x00100c04,
    RCH_TRIM_8M = 0x00100c06,
    RCH_TRIM_4M = 0x00100c08,
};

enum XTH_STARTUP_CYCLE_NUM {
    XTH_STARTUP_256 = 0,
    XTH_STARTUP_1024,
    XTH_STARTUP_4096,
    XTH_STARTUP_16384,
};

enum XTH_FREQ {
    XTH_FREQ_4_6 = 0,
    XTH_FREQ_6_12,
    XTH_FREQ_12_20,
    XTH_FREQ_20_32,
};

enum XTH_DRIVER_STRENGTH {
    XTH_DRIVER_STRENGTH_LOWEST = 0,
    XTH_DRIVER_STRENGTH_LOW,
    XTH_DRIVER_STRENGTH_DEFAULT,
    XTH_DRIVER_STRENGTH_HIGHEST,
};

enum RCL_STARTUP_CYCLE_NUM {
    RCH_STARTUP_4 = 0,
    RCH_STARTUP_16,
    RCH_STARTUP_64,
    RCH_STARTUP_256,
};

enum RCL_TRIM {
    RCL_TRIM_38K = 0x00100c20,
    RCL_TRIM_32K = 0x00100c22,
};

enum XTL_STARTUP_CYCLE_NUM {
    XTL_STARTUP_256 = 0,
    XTL_STARTUP_1024,
    XTL_STARTUP_4096,
    XTL_STARTUP_16384,
};

enum XTL_AMP_CTRL {
    XTL_AMP_MIN = 0,
    XTL_AMP_MAX,
};

enum XTL_DRIVER_STRENGTH {
    XTL_DRIVER_STRENGTH_LOWEST = 0,
    XTL_DRIVER_STRENGTH_DEFAULT,
    XTL_DRIVER_STRENGTH_HIGH,
    XTL_DRIVER_STRENGTH_HIGHEST,
};

enum SYS_CLK_SRC {
    SYS_CLK_SRC_RCH = 0,
    SYS_CLK_SRC_XTH,
    SYS_CLK_SRC_RCL,
    SYS_CLK_SRC_XTL,
};

enum HCLK_SRC {
    HCLK_SYS_CLK_DIV_1 = 0,
    HCLK_SYS_CLK_DIV_2,
    HCLK_SYS_CLK_DIV_4,
    HCLK_SYS_CLK_DIV_8,
    HCLK_SYS_CLK_DIV_16,
    HCLK_SYS_CLK_DIV_32,
    HCLK_SYS_CLK_DIV_64,
    HCLK_SYS_CLK_DIV_128,
};

enum PCLK_SRC {
    PCLK_HCLK_DIV_1 = 0,
    PCLK_HCLK_DIV_2,
    PCLK_HCLK_DIV_4,
    PCLK_HCLK_DIV_8,
};

enum WAKEUP_CLK_SRC {
    WAKEUP_CLK_SRC_NOCHANGE = 0,
    WAKEUP_CLK_SRC_RCH,
};

enum PERI_CLK {
    PERI_CLK_UART0 = 0,
    PERI_CLK_UART1 = 1,
    PERI_CLK_I2C = 4,
    PERI_CLK_SPI = 6,
    PERI_CLK_BT = 8,
    PERI_CLK_ADT = 10,
    PERI_CLK_PCA = 14,
    PERI_CLK_WDT = 15,
    PERI_CLK_ADC = 16,
    PERI_CLK_VC_LVD = 17,
    PERI_CLK_TRIM = 21,
    PERI_CLK_TICK_CONTROLLER = 24,
    PERI_CLK_CRC = 26,
    PERI_CLK_GPIO = 28,
    PERI_CLK_FLASH_CONTROLLER = 31,
};

enum SYSTICK_CLK_SRC {
    SYSTICK_CLK_SRC_XTL = 0,
    SYSTICK_CLK_SRC_RCL,
    SYSTICK_CLK_SRC_SCLK_DIV_8,
    SYSTICK_CLK_SRC_XTH,
    SYSTICK_CLK_SRC_SCLK,
};

void sysctl_deinit();
void sysctl_rch_config(enum RCH_TRIM);
void sysctl_rch_enable();
void sysctl_rch_disable();
bool sysctl_rch_stable();
void sysctl_xth_config(enum XTH_STARTUP_CYCLE_NUM, enum XTH_FREQ,
                       enum XTH_DRIVER_STRENGTH);
void sysctl_xth_enable();
void sysctl_xth_disable();
bool sysctl_xth_stable();
void sysctl_rcl_config(enum RCL_STARTUP_CYCLE_NUM, enum RCL_TRIM);
void sysctl_rcl_enable();
void sysctl_rcl_disable();
bool sysctl_rcl_stable();
void sysctl_xtl_config(enum XTL_STARTUP_CYCLE_NUM, enum XTL_AMP_CTRL,
                       enum XTL_DRIVER_STRENGTH);
void sysctl_xtl_enable();
void sysctl_xtl_disable();
bool sysctl_xtl_stable();
void sysctl_system_clock_config(enum SYS_CLK_SRC);
void sysctl_hclk_config(enum HCLK_SRC);
void sysctl_pclk_config(enum PCLK_SRC);
void sysctl_wakeup_clk_config(enum WAKEUP_CLK_SRC);
void sysctl_swd_as_io(bool);
void sysctl_rst_as_io(bool);
void sysctl_cm0plus_lockup_en(bool);
void sysctl_clk_fault_en(bool);
void sysctl_xtl_always_on(bool);
void sysctl_exth_en(bool);
void sysctl_extl_en(bool);
void sysctl_peri_clock_enable(enum PERI_CLK);
void sysctl_peri_clock_disable(enum PERI_CLK);
void sysctl_systick_config(enum SYSTICK_CLK_SRC);

#endif // __HC32F00X_SYSCTL_H__
