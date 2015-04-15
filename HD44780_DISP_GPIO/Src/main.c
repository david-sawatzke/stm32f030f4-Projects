
#include "main.h"


DISP_HandleTypeDef DISP_HandleStruct;

void SystemClock_Config(void);

int main(void)
{
    int i = 0;
    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_Delay(500);
    DISP_HandleStruct.D4.Port = GPIOA;
    DISP_HandleStruct.D4.Pin = GPIO_PIN_5;
    DISP_HandleStruct.D5.Port = GPIOA;
    DISP_HandleStruct.D5.Pin = GPIO_PIN_6;
    DISP_HandleStruct.D6.Port = GPIOA;
    DISP_HandleStruct.D6.Pin = GPIO_PIN_7;
    DISP_HandleStruct.D7.Port = GPIOB;
    DISP_HandleStruct.D7.Pin = GPIO_PIN_1;
    DISP_HandleStruct.RS.Port = GPIOA;
    DISP_HandleStruct.RS.Pin = GPIO_PIN_9;
    DISP_HandleStruct.E.Port = GPIOA;
    DISP_HandleStruct.E.Pin = GPIO_PIN_10;
    DISP_HandleStruct.Lines = 2;
    DISP_Init(&DISP_HandleStruct);
    DISP_Puts(&DISP_HandleStruct, "Hello World!");
    DISP_Setcursor(&DISP_HandleStruct, 0, 2);
    DISP_Puts(&DISP_HandleStruct, "Bye, cruel World!");
    HAL_Delay(1000);
    DISP_CMD(&DISP_HandleStruct, DISP_CLR);
    HAL_Delay(1);
    while (1) {
        DISP_Setcursor(&DISP_HandleStruct, 0, 1);
        DISP_PutInt(&DISP_HandleStruct, i);
        i++;
        HAL_Delay(10);
    }
}


void SystemClock_Config(void)
{

}
