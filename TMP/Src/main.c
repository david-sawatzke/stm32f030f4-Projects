#include "main.h"


GPIO_InitTypeDef  GPIO_InitStruct;
TIM_HandleTypeDef TIM_HandleStruct;
TIM_OC_InitTypeDef TIM_OC_InitStruct;
uint8_t Counter = 0;

static void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    /* Just a Stub for future changes */
    SystemClock_Config();
    
    
    /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
    __GPIOA_CLK_ENABLE();
    __TIM14_CLK_ENABLE();
    /* -2- Configure IOs in output push-pull mode to drive external LEDs */
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* -3- Configure Timer */
    TIM_HandleStruct.Instance = TIM14;
    TIM_HandleStruct.Init.Prescaler = 0;
    TIM_HandleStruct.Init.Period = 0xF; //8bit
    TIM_HandleStruct.Init.ClockDivision = 1;
    TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_HandleStruct.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&TIM_HandleStruct);
    /* -4- Configure PWM Functionality of timer */
    TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
    TIM_OC_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;
    TIM_OC_InitStruct.Pulse = Counter;
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct,&TIM_OC_InitStruct,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TIM_HandleStruct,TIM_CHANNEL_1);
/* -3- Toggle IOs in an infinite loop */
    while (1)
    {
	Counter ++;
	if(Counter == 0x10){Counter = 0;}
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
	TIM_OC_InitStruct.Pulse = Counter;
	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct,&TIM_OC_InitStruct,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_HandleStruct,TIM_CHANNEL_1);
    }
}

static void SystemClock_Config(void)
{
}

