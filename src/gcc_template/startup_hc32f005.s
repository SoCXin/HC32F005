.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

.section  .text.Reset_Handler
.weak  Reset_Handler
.type  Reset_Handler, %function
Reset_Handler:  
    ldr   r0, =_estack
    mov   sp, r0          /* set stack pointer */

ApplicationStart:
/* Copy the data segment initializers from flash to SRAM */
    movs  r1, #0
    b  LoopCopyDataInit

CopyDataInit:
    ldr  r3, =_sidata
    ldr  r3, [r3, r1]
    str  r3, [r0, r1]
    adds  r1, r1, #4

LoopCopyDataInit:
    ldr  r0, =_sdata
    ldr  r3, =_edata
    adds  r2, r0, r1
    cmp  r2, r3
    bcc  CopyDataInit
    ldr  r2, =_sbss
    b  LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
    movs  r3, #0
    str  r3, [r2]
    adds r2, r2, #4


LoopFillZerobss:
    ldr  r3, = _ebss
    cmp  r2, r3
    bcc  FillZerobss

/* From Vendor.*/
/* reset NVIC if in rom debug.*/
    LDR     R0, =0x20000000
    LDR     R2, =0x0
    MOVS    R1, #0
    ADD     R1, PC,#0
    CMP     R1, R0
    BLS     RAMCODE

/* ram code base address.*/
    ADD     R2, R0,R2
RAMCODE:
/* reset Vector table address.*/
    LDR     R0, =0xE000ED08
    STR     R2, [R0]
/* End From Vendor.*/

/* Call the clock system intitialization function.*/
    bl  SystemInit
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
    bl  main

LoopForever:
    b LoopForever


.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
    .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0+.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word 0
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler
/* external interrupts handler */
	.word PORT0_IRQHandler
    .word PORT1_IRQHandler
    .word PORT2_IRQHandler
    .word PORT3_IRQHandler
    .word Dummy_Handler
    .word Dummy_Handler
    .word UART0_IRQHandler
    .word UART1_IRQHandler
    .word Dummy_Handler
    .word Dummy_Handler
    .word SPI_IRQHandler
    .word Dummy_Handler
    .word I2C_IRQHandler
    .word Dummy_Handler
    .word TIM0_IRQHandler
    .word TIM1_IRQHandler
    .word TIM2_IRQHandler
    .word Dummy_Handler
    .word TIM4_IRQHandler
    .word TIM5_IRQHandler
    .word TIM6_IRQHandler
    .word PCA_IRQHandler
    .word WDT_IRQHandler
    .word Dummy_Handler
    .word ADC_IRQHandler
    .word Dummy_Handler
    .word VC0_IRQHandler
    .word VC1_IRQHandler
    .word LVD_IRQHandler
    .word Dummy_Handler
    .word EF_RAM_IRQHandler
    .word CLKTRIM_IRQHandler

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

    .weak NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

/* external interrupts handler */
	
	.weak PORT0_IRQHandler
    .thumb_set PORT0_IRQHandler,Default_Handler
    .weak PORT1_IRQHandler
    .thumb_set PORT1_IRQHandler,Default_Handler
    .weak PORT2_IRQHandler
    .thumb_set PORT2_IRQHandler,Default_Handler
    .weak PORT3_IRQHandler
    .thumb_set PORT3_IRQHandler,Default_Handler
    .weak UART0_IRQHandler
    .thumb_set UART0_IRQHandler,Default_Handler
    .weak UART1_IRQHandler
    .thumb_set UART1_IRQHandler,Default_Handler
    .weak SPI_IRQHandler
    .thumb_set SPI_IRQHandler,Default_Handler
    .weak I2C_IRQHandler
    .thumb_set I2C_IRQHandler,Default_Handler
    .weak TIM0_IRQHandler
    .thumb_set TIM0_IRQHandler,Default_Handler
    .weak TIM1_IRQHandler
    .thumb_set TIM1_IRQHandler,Default_Handler
    .weak TIM2_IRQHandler
    .thumb_set TIM2_IRQHandler,Default_Handler
    .weak TIM4_IRQHandler
    .thumb_set TIM4_IRQHandler,Default_Handler
    .weak TIM5_IRQHandler
    .thumb_set TIM5_IRQHandler,Default_Handler
    .weak TIM6_IRQHandler
    .thumb_set TIM6_IRQHandler,Default_Handler
    .weak PCA_IRQHandler
    .thumb_set PCA_IRQHandler,Default_Handler
    .weak WDT_IRQHandler
    .thumb_set WDT_IRQHandler,Default_Handler
    .weak ADC_IRQHandler
    .thumb_set ADC_IRQHandler,Default_Handler
    .weak VC0_IRQHandler
    .thumb_set VC0_IRQHandler,Default_Handler
    .weak VC1_IRQHandler
    .thumb_set VC1_IRQHandler,Default_Handler
    .weak LVD_IRQHandler
    .thumb_set LVD_IRQHandler,Default_Handler
    .weak EF_RAM_IRQHandler
    .thumb_set EF_RAM_IRQHandler,Default_Handler
    .weak CLKTRIM_IRQHandler
    .thumb_set CLKTRIM_IRQHandler,Default_Handler
    .weak Dummy_Handler
    .thumb_set Dummy_Handler,Default_Handler
