//msp.c

//All the APIs related to MSP are located to "stm32f1xx_hal_cortex.c"

#include "stm32f1xx_hal.h"

void HAL_MspInit(void)
{
	//1. Set Priority Grouping ,  "stm32f1xx_hal_cortex.c"
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);  //(default)  4 bits preemption priority & 0 bits subpriority [#1]
	//2. Enable M3-specific Exceptions  ,  "Cortex-M3 Devices Generc User Guide.pdf" pg.34 & p.149
	//Enable USGFAULTENA, BUSFAULTENA, MEMFAULTENA  18,17,16 bits of SHCSR Reg.
	SCB->SHCSR |= (0x7 << 16);  // 1 1 1 starting from bit 16
	//3. Set the Priority of System Exceptions (optional)
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 1, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 2, 0);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//1. Enable the Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	//2. Do the pin-muxing configuration
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3. Enable the IRQ and setup the NVIC priority (optional)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);  //See [#1] , 15 is the lowest priority of the 4 bits allowed
}

