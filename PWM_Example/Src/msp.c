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

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	 GPIO_InitTypeDef tim2OC_ch_gpios;
	 //1. enable the peripheral clock for the timer2 peripheral
	 __HAL_RCC_TIM2_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();

	 /*2. Configure gpios to behave as timer2 channel 1,2,3 and 4
	 PA0 --> TIM2_CH1
	 PA1 --> TIM2_CH2
	 PB10 --> TIM2_CH3
	 PB2 --> TIM2_CH4 */

	tim2OC_ch_gpios.Pin = GPIO_PIN_0|GPIO_PIN_1;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Pull = GPIO_PULLDOWN;
	tim2OC_ch_gpios.Speed = GPIO_SPEED_FREQ_LOW;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2OC_ch_gpios);

	tim2OC_ch_gpios.Pin = GPIO_PIN_2|GPIO_PIN_10;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Pull = GPIO_PULLDOWN;
	tim2OC_ch_gpios.Speed = GPIO_SPEED_FREQ_LOW;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &tim2OC_ch_gpios);

	 //3. nvic settings
	 HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	 HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
