//msp.c

#include "main.h"

//Low level processor initializations
void HAL_MspInit(void)
{
	//1. Setup priority grouping (stm32f4xx_hal_cortex.c)
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //No SubPriority

	//2. Enable system exceptions
	//   List at 'Cortex-M4 Generic User Guide.pdf' p.34
	//	 Find image 'System Handler Control Register.jpg'
	//   Documentation at Cortex-M4 Generic User Guide.pdf' p.239
	//   We have to dereference the System Control Block to System Handler Control and State Register
	SCB->SHCSR |= 0x7 << 16;   //111 at bit 16,17,18 for MemManage, BusFault, UsageFault

	//3. Configure priority of the system exceptions [stm32f4xx_hal_cortex.c]
	//   In accordance with the 1. (priority grouping where the 3rd arg. is 0; No Subpriority)
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 1, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 2, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	 GPIO_InitTypeDef gpio_uart;
	 //here we are going to do the low level inits. of the USART2 peripheral

	 //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
	 __HAL_RCC_USART2_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	 //2 . Do the pin muxing configurations
	 gpio_uart.Pin = GPIO_PIN_2;
	 gpio_uart.Mode =GPIO_MODE_AF_PP;
	 gpio_uart.Pull = GPIO_PULLUP;
	 gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	 gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);

	 gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);
	 //3 . Enable the IRQ and set up the priority (NVIC settings )
	 HAL_NVIC_EnableIRQ(USART2_IRQn);
	 HAL_NVIC_SetPriority(USART2_IRQn,15,0);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_CAN1_CLK_ENABLE();
	//CAN1 GPIO Configuration	PA11     ------> CAN1_RX
	//							PA12     ------> CAN1_TX
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//Set CAN's IRQ Priority
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);
	//Enable CAN's IRQs
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
 }

