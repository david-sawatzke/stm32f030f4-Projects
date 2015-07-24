#include "main.h"


static GPIO_InitTypeDef  GPIO_InitStruct;

static void SystemClock_Config(void);

int main(void)
{
	/* STM32F0xx HAL library initialization:
	- Configure the Flash prefetch
	- Systick timer is configured by default as source of time base, but user 
	can eventually implement his proper time base source (a general purpose 
	timer for example or other time source), keeping in mind that Time base 
	duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
	handled in milliseconds basis.
	- Low Level Initialization
	*/
	HAL_Init();
	
	/* Just a Stub for future changes */
	SystemClock_Config();
	
	/* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
	__GPIOA_CLK_ENABLE();
	/* -2- Configure IOs in output push-pull mode to drive external LEDs */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* -3- Cobfigure PA3 in Input Mode with Pull-up */
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* -4- Activate LED on Buttonpress */
	while (1){
	  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3) == GPIO_PIN_RESET){
	      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	  }else{
	      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
	  }
	}
}

static void SystemClock_Config(void)
{
	
}
