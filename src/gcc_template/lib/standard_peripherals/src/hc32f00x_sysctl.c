#include "hc32f00x_sysctl.h"
#include "hc32f005.h"

void sysctl_0_1_we() {
    M0P_CLOCK->SYSCTRL2 = 0x5a5a;
    M0P_CLOCK->SYSCTRL2 = 0xa5a5;
}

void sysctl_deinit() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0 = 0x00000001;
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1 = 0x00000008;
    M0P_CLOCK->RCH_CR_f.TRIM = 0x126;
    M0P_CLOCK->XTH_CR_f.STARTUP = 0x2;
    M0P_CLOCK->XTH_CR_f.DRIVER = 0x2;
    M0P_CLOCK->RCL_CR_f.STARTUP = 0x0;
    M0P_CLOCK->RCL_CR_f.TRIM = 0x33f;
    M0P_CLOCK->XTL_CR_f.STARTUP = 0x2;
    M0P_CLOCK->XTL_CR_f.DRIVER = 0x1;
    M0P_CLOCK->PERI_CLKEN = 0xc0800000;
    M0P_CLOCK->SYSTICK_CR = 0x01000147;
}

void sysctl_rch_config(enum RCH_TRIM trim_val) {
    M0P_CLOCK->RCH_CR_f.STABLE = trim_val;
}

void sysctl_rch_enable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.RCH_EN = 1;
}

void sysctl_rch_disable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.RCH_EN = 0;
}

bool sysctl_rch_stable() { return M0P_CLOCK->RCH_CR_f.STABLE; }

void sysctl_xth_config(enum XTH_STARTUP_CYCLE_NUM cy_num, enum XTH_FREQ freq,
                       enum XTH_DRIVER_STRENGTH str) {
    M0P_CLOCK->XTH_CR_f.STARTUP = cy_num;
    M0P_CLOCK->XTH_CR_f.DRIVER = (freq << 2) | str;
}

void sysctl_xth_enable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.XTH_EN = 1;
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTH_EN = 1;
}

void sysctl_xth_disable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.XTH_EN = 0;
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTH_EN = 0;
}

bool sysctl_xth_stable() { return M0P_CLOCK->XTH_CR_f.STABLE; }

void sysctl_rcl_config(enum RCL_STARTUP_CYCLE_NUM cy_num,
                       enum RCL_TRIM trim_val) {
    M0P_CLOCK->RCL_CR_f.STARTUP = cy_num;
    M0P_CLOCK->RCL_CR_f.TRIM = trim_val;
}

void sysctl_rcl_enable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.RCL_EN = 1;
}

void sysctl_rcl_disable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.RCL_EN = 0;
}

bool sysctl_rcl_stable() { return M0P_CLOCK->RCL_CR_f.STABLE; }

void sysctl_xtl_config(enum XTL_STARTUP_CYCLE_NUM cy_num,
                       enum XTL_AMP_CTRL amp_cfg,
                       enum XTL_DRIVER_STRENGTH str) {
    M0P_CLOCK->XTL_CR_f.STARTUP = cy_num;
    M0P_CLOCK->XTH_CR_f.DRIVER = (amp_cfg << 2) | str;
}

void sysctl_xtl_enable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.XTL_EN = 1;
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTL_EN = 1;
}

void sysctl_xtl_disable() {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.XTL_EN = 0;
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTL_EN = 0;
}

bool sysctl_xtl_stable() { return M0P_CLOCK->XTL_CR_f.STABLE; }

void sysctl_system_clock_config(enum SYS_CLK_SRC src) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.CLK_SW4_SEL = src;
}

void sysctl_hclk_config(enum HCLK_SRC src) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.HCLK_PRS = src;
}

void sysctl_pclk_config(enum PCLK_SRC src) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.PCLK_PRS = src;
}

void sysctl_wakeup_clk_config(enum WAKEUP_CLK_SRC src) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL0_f.WAKEUP_BYRCH = src;
}

void sysctl_swd_as_io(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.SWD_UIO = b;
}

void sysctl_rst_as_io(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.RES_UIO = b;
}

void sysctl_cm0plus_lockup_en(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.LOCK_EN = b;
}

void sysctl_clk_fault_en(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.CLOCK_FT_EN = b;
}

void sysctl_xtl_always_on(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.XTL_ALWAYSON = b;
}

void sysctl_exth_en(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTH_EN = b;
}

void sysctl_extl_en(bool b) {
    sysctl_0_1_we();
    M0P_CLOCK->SYSCTRL1_f.EXTL_EN = b;
}

void sysctl_peri_clock_enable(enum PERI_CLK peri_clk) {
    M0P_CLOCK->PERI_CLKEN |= (1 << peri_clk);
}

void sysctl_peri_clock_disable(enum PERI_CLK peri_clk) {
    M0P_CLOCK->PERI_CLKEN &= ~(1 << peri_clk);
}

void sysctl_systick_config(enum SYSTICK_CLK_SRC src) {
    if (src == SYSTICK_CLK_SRC_SCLK) {
        M0P_CLOCK->SYSTICK_CR_f.NOREF = 1;
    } else {
        M0P_CLOCK->SYSTICK_CR_f.NOREF = 0;
        M0P_CLOCK->SYSTICK_CR_f.CLK_SEL = src;
    }
}
