//main.c

#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"

void SystemClock_Config_HSE(uint8_t clock_freq);
void Error_Handler(void);
void GPIO_Init(void);
void UART2_Init(void);
void TIMER2_Init(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;

int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();
	//Start PWM channels
	if( HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1) != HAL_OK ) Error_Handler();
	if( HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_2) != HAL_OK ) Error_Handler();
	if( HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_3) != HAL_OK ) Error_Handler();
	if( HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_4) != HAL_OK ) Error_Handler();
	while(1);
	return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;
	uint8_t flash_latency=0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 1;
	  break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 2;
	  break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
          flash_latency = 3;
	  break;

	  default:
	  return ;
	}
	if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK) Error_Handler();
	if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK) Error_Handler();

	//Configure the systick timer interrupt frequency (for every 1 ms)
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);
	//Configure the Systick
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void Error_Handler(void)
{
	while(1);
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK ) Error_Handler();
}

void GPIO_Init(void)
{
	GPIO_InitTypeDef ledgpio;
	__HAL_RCC_GPIOA_CLK_ENABLE();   //Enables GPIO port clock
	ledgpio.Pin = GPIO_PIN_5;	//port A Pin 5 for the LED on board
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;    //optional, this is the default value
	HAL_GPIO_Init(GPIOA, &ledgpio);    //As such because it does not return anything
}

void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef tim2PWM_Config;
	htimer2.Instance = TIM2;
	//Produce period of 1 sec
	htimer2.Init.Period = 10000-1;    //ARR value
	htimer2.Init.Prescaler = 4999;
	if ( HAL_TIM_PWM_Init(&htimer2) != HAL_OK ) Error_Handler();
	memset(&tim2PWM_Config, 0, sizeof(tim2PWM_Config));	//clear the struct
	tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;    //Non-inverting mode, High=1
	//PWM creation
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 0.25);  //25% of the period
	if( HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_1) != HAL_OK ) Error_Handler();
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 0.45);  //45% of the period
	if( HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_2) != HAL_OK ) Error_Handler();
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 0.75);  //75% of the period
	if( HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_3) != HAL_OK ) Error_Handler();
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 0.90);  //90% of the period
	if( HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_4) != HAL_OK ) Error_Handler();
}
