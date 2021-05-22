
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "FreeRTOS.h"
#include "HC32F005.h"
#include "task.h"

void idle_task(void *param) {
    while (1) {
        __asm volatile("nop");
        vTaskDelay(1000);
    }
}

int main(void) {
    xTaskCreate(idle_task, "idle_task", configMINIMAL_STACK_SIZE, 0, 0, 0);
    vTaskStartScheduler();
}
