
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "FreeRTOS.h"
#include "HC32F005.h"
#include "task.h"

void idle_task(void *param) {
    while (1) {
        __asm volatile("nop");
        vTaskDelay(500);
        // Gpio_WriteOutputIO(GpioPort0, GpioPin3, 0);
        // vTaskDelay(500);
        // Gpio_WriteOutputIO(GpioPort0, GpioPin3, 1);
    }
}

int main(void)
{
    // sysctl_peri_clock_enable(PERI_CLK_GPIO);
    xTaskCreate(idle_task, "idle_task", configMINIMAL_STACK_SIZE, 0, 0, 0);
    vTaskStartScheduler();
}
