
#include "main.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_OC_InitTypeDef sConfig;

static void SystemClock_Config(void);

uint32_t uhPrescalerValue = 0;

int main(void)
{

    HAL_Init();
    SystemClock_Config();
    //We want the Timer to run with 8MHz (the maximum, since we're running of the internal Clock, which runs at 8MHz, without PLL)
    uhPrescalerValue = (uint32_t)(SystemCoreClock / 8000000) - 1;

    while (1) {

    }
}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    // Internal Oscillator Directly (it's fairly accurate, at least better than the AVR one
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

}
