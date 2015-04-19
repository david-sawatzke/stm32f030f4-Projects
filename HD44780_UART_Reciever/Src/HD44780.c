#include "HD44780.h"


void HD44780_PutInt(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, int pData)
{
    if (pData < 0) {
        HD44780_Putc(HD44780_HandleFunctionStruct, '-');
        pData *= -1;
    } else {
        HD44780_Putc(HD44780_HandleFunctionStruct, '+');
    }
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + (pData / 1000000000)); //billion
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 100000000) % 10)); //hundred million
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 10000000) % 10)); //ten million
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 1000000) % 10)); //million
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 100000) % 10)); //hundred thousand
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 10000) % 10)); //hundred thousand
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 1000) % 10)); //thousand
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 100) % 10)); //hundred
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + ((pData / 10) % 10)); //ten
    HD44780_Putc(HD44780_HandleFunctionStruct, '0' + (pData % 10)); //one

}

HAL_StatusTypeDef HD44780_Init(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //Enable Clock for all Pins
    HD44780_GPIO_Clk_Init(HD44780_HandleFunctionStruct);
    //Set All Pins as Output
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->D4.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->D4.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->D5.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->D5.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->D6.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->D6.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->D7.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->D7.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->RS.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->RS.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = HD44780_HandleFunctionStruct->E.Pin;
    HAL_GPIO_Init(HD44780_HandleFunctionStruct->E.Port, &GPIO_InitStruct);
    //Control Pins High
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->RS.Port, HD44780_HandleFunctionStruct->RS.Pin, GPIO_PIN_RESET);
    //Data Lines Low
    HD44780_Out(HD44780_HandleFunctionStruct, 0x00);
    //Startup Delay
    HAL_Delay(15);
    //Init Display
    //soft-reset
    HD44780_Out(HD44780_HandleFunctionStruct, HD44780_RST);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(5);

    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    //4-bit Mode
    HD44780_Out(HD44780_HandleFunctionStruct, HD44780_FUNCTION);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(5);

    if (HD44780_HandleFunctionStruct->Lines < 1) {
        HD44780_CMD(HD44780_HandleFunctionStruct, HD44780_FUNCTION | HD44780_FUNCTION_2LINES);
    }

    //Display On, CURSOR and Blink off
    HD44780_CMD(HD44780_HandleFunctionStruct, HD44780_HD44780LAYMODE | HD44780_HD44780LAYMODE_ON);
    //Entry Mode Set
    HD44780_CMD(HD44780_HandleFunctionStruct, HD44780_ENTRY_MODE | HD44780_ENTRY_INC);
    HD44780_CMD(HD44780_HandleFunctionStruct, HD44780_CLR);
    HAL_Delay(2);

    return HAL_OK;
}

void HD44780_GPIO_Clk_Init(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct)
{
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOA) || (HD44780_HandleFunctionStruct->D5.Port == GPIOA) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOA) || (HD44780_HandleFunctionStruct->D7.Port == GPIOA) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOA) || (HD44780_HandleFunctionStruct->E.Port == GPIOA)) {
        __GPIOA_CLK_ENABLE();
    }
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOB) || (HD44780_HandleFunctionStruct->D5.Port == GPIOB) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOB) || (HD44780_HandleFunctionStruct->D7.Port == GPIOB) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOB) || (HD44780_HandleFunctionStruct->E.Port == GPIOB)) {
        __GPIOB_CLK_ENABLE();
    }
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOC) || (HD44780_HandleFunctionStruct->D5.Port == GPIOC) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOC) || (HD44780_HandleFunctionStruct->D7.Port == GPIOC) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOC) || (HD44780_HandleFunctionStruct->E.Port == GPIOC)) {
        __GPIOC_CLK_ENABLE();
    }
#if defined(STM32F030x6) || defined(STM32F030x8) || \
    defined(STM32F051x8) || defined(STM32F058xx) || defined(STM32F070xB) || \
    defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F030xC)
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOD) || (HD44780_HandleFunctionStruct->D5.Port == GPIOD) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOD) || (HD44780_HandleFunctionStruct->D7.Port == GPIOD) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOD) || (HD44780_HandleFunctionStruct->E.Port == GPIOD)) {
        __GPIOD_CLK_ENABLE();
    }
#endif
#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F070xB) || \
    defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F030xC)
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOE) || (HD44780_HandleFunctionStruct->D5.Port == GPIOE) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOE) || (HD44780_HandleFunctionStruct->D7.Port == GPIOE) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOE) || (HD44780_HandleFunctionStruct->E.Port == GPIOE)) {
        __GPIOE_CLK_ENABLE();
    }
#endif
    if ((HD44780_HandleFunctionStruct->D4.Port == GPIOF) || (HD44780_HandleFunctionStruct->D5.Port == GPIOF) ||
            (HD44780_HandleFunctionStruct->D6.Port == GPIOF) || (HD44780_HandleFunctionStruct->D7.Port == GPIOF) ||
            (HD44780_HandleFunctionStruct->RS.Port == GPIOF) || (HD44780_HandleFunctionStruct->E.Port == GPIOF)) {
        __GPIOF_CLK_ENABLE();
    }
}

void HD44780_CMD(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t cmd)
{
    HD44780_Write(HD44780_HandleFunctionStruct, cmd, 0);
}

void HD44780_Puts(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, char *data)
{
    while (*data != '\0') {
        HD44780_Putc(HD44780_HandleFunctionStruct, *data++);
    }
}

void HD44780_Putc(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, char c)
{
    HD44780_Write(HD44780_HandleFunctionStruct, c, 1);
}

void HD44780_Out(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t data)
{
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->D7.Port, HD44780_HandleFunctionStruct->D7.Pin, data & (1 << 7));
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->D6.Port, HD44780_HandleFunctionStruct->D6.Pin, data & (1 << 6));
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->D5.Port, HD44780_HandleFunctionStruct->D5.Pin, data & (1 << 5));
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->D4.Port, HD44780_HandleFunctionStruct->D4.Pin, data & (1 << 4));
}

//WARNING: If you don't have 8MHz as your clock frequency, change the Delay values
void HD44780_Write(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t data, uint8_t rs)
{
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->RS.Port, HD44780_HandleFunctionStruct->RS.Pin, rs);

    HD44780_Out(HD44780_HandleFunctionStruct, data);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HD44780_Delay(50);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);

    HD44780_Out(HD44780_HandleFunctionStruct, (data << 4));
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_SET);
    HD44780_Delay(50);
    HAL_GPIO_WritePin(HD44780_HandleFunctionStruct->E.Port, HD44780_HandleFunctionStruct->E.Pin, GPIO_PIN_RESET);
    HD44780_Delay(20);

    HD44780_Out(HD44780_HandleFunctionStruct, 0xff);
}

void HD44780_Setcursor(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t x, uint8_t y)
{
    uint8_t data;

    switch (y) {
    case 0: // 0. Line
        data = HD44780_DDRAM + HD44780_DDADR_LINE1 + x;
        break;

    case 1: // 1. Line
        data = HD44780_DDRAM + HD44780_DDADR_LINE2 + x;
        break;

    case 2: // 2. Line
        data = HD44780_DDRAM + HD44780_DDADR_LINE3 + x;
        break;

    case 3: // 3. Line
        data = HD44780_DDRAM + HD44780_DDADR_LINE4 + x;
        break;

    default:
        return;
    }

    HD44780_CMD(HD44780_HandleFunctionStruct, data);
}

void HD44780_Delay(uint16_t t)
{
    volatile uint16_t i;
    for (i = 0; i <= t; i++);
}
