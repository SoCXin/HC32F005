
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "FreeRTOS.h"
#include "HC32F005.h"
#include "task.h"

static void App_LedInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    stcGpioCfg.enDir = GpioDirOut;
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);
    Gpio_Init(GpioPort0, GpioPin3, &stcGpioCfg);
}

void idle_task(void *param) {
    while (1) {
        __asm volatile("nop");
        vTaskDelay(500);
        Gpio_WriteOutputIO(GpioPort0, GpioPin3, FALSE);
        vTaskDelay(500);
        Gpio_WriteOutputIO(GpioPort0, GpioPin3, TRUE);
    }
}

int main(void) {
    App_LedInit();
    xTaskCreate(idle_task, "idle_task", configMINIMAL_STACK_SIZE, 0, 0, 0);
    vTaskStartScheduler();
}
