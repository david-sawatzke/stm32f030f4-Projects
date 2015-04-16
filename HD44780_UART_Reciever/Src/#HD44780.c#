#include "HD44780.h"


void DISP_PutInt(DISP_HandleTypeDef *DISP_HandleStruct, int pData)
{
    if (pData < 0) {
        DISP_Putc(DISP_HandleStruct, '-');
        pData *= -1;
    } else {
        DISP_Putc(DISP_HandleStruct, '+');
    }
    DISP_Putc(DISP_HandleStruct, '0' + (pData / 1000000000)); //billion
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 100000000) % 10)); //hundred million
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 10000000) % 10)); //ten million
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 1000000) % 10)); //million
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 100000) % 10)); //hundred thousand
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 10000) % 10)); //hundred thousand
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 1000) % 10)); //thousand
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 100) % 10)); //hundred
    DISP_Putc(DISP_HandleStruct, '0' + ((pData / 10) % 10)); //ten
    DISP_Putc(DISP_HandleStruct, '0' + (pData % 10)); //one

}

HAL_StatusTypeDef DISP_Init(DISP_HandleTypeDef *DISP_HandleStruct)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //Enable Clock for all Pins
    DISP_GPIO_Clk_Init(DISP_HandleStruct);
    //Set All Pins as Output
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pin = DISP_HandleStruct->D4.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->D4.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_HandleStruct->D5.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->D5.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_HandleStruct->D6.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->D6.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_HandleStruct->D7.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->D7.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_HandleStruct->RS.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->RS.Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_HandleStruct->E.Pin;
    HAL_GPIO_Init(DISP_HandleStruct->E.Port, &GPIO_InitStruct);
    //Control Pins High
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DISP_HandleStruct->RS.Port, DISP_HandleStruct->RS.Pin, GPIO_PIN_RESET);
    //Data Lines Low
    DISP_Out(DISP_HandleStruct, 0x00);
    //Startup Delay
    HAL_Delay(15);
    //Init Display
    //soft-reset
    DISP_Out(DISP_HandleStruct, DISP_RST);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(5);

    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    //4-bit Mode
    DISP_Out(DISP_HandleStruct, DISP_FUNCTION);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(5);

    if (DISP_HandleStruct->Lines < 1) {
        DISP_CMD(DISP_HandleStruct, DISP_FUNCTION | DISP_FUNCTION_2LINES);
    }

    //Display On, CURSOR and Blink off
    DISP_CMD(DISP_HandleStruct, DISP_DISPLAYMODE | DISP_DISPLAYMODE_ON);
    //Entry Mode Set
    DISP_CMD(DISP_HandleStruct, DISP_ENTRY_MODE | DISP_ENTRY_INC);
    DISP_CMD(DISP_HandleStruct, DISP_CLR);
    HAL_Delay(2);

    return HAL_OK;
}

void DISP_GPIO_Clk_Init(DISP_HandleTypeDef *DISP_HandleStruct)
{
    if ((DISP_HandleStruct->D4.Port == GPIOA) || (DISP_HandleStruct->D5.Port == GPIOA) ||
            (DISP_HandleStruct->D6.Port == GPIOA) || (DISP_HandleStruct->D7.Port == GPIOA) ||
            (DISP_HandleStruct->RS.Port == GPIOA) || (DISP_HandleStruct->E.Port == GPIOA)) {
        __GPIOA_CLK_ENABLE();
    }
    if ((DISP_HandleStruct->D4.Port == GPIOB) || (DISP_HandleStruct->D5.Port == GPIOB) ||
            (DISP_HandleStruct->D6.Port == GPIOB) || (DISP_HandleStruct->D7.Port == GPIOB) ||
            (DISP_HandleStruct->RS.Port == GPIOB) || (DISP_HandleStruct->E.Port == GPIOB)) {
        __GPIOB_CLK_ENABLE();
    }
    if ((DISP_HandleStruct->D4.Port == GPIOC) || (DISP_HandleStruct->D5.Port == GPIOC) ||
            (DISP_HandleStruct->D6.Port == GPIOC) || (DISP_HandleStruct->D7.Port == GPIOC) ||
            (DISP_HandleStruct->RS.Port == GPIOC) || (DISP_HandleStruct->E.Port == GPIOC)) {
        __GPIOC_CLK_ENABLE();
    }
#if defined(STM32F030x6) || defined(STM32F030x8) || \
    defined(STM32F051x8) || defined(STM32F058xx) || defined(STM32F070xB) || \
    defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F030xC)
    if ((DISP_HandleStruct->D4.Port == GPIOD) || (DISP_HandleStruct->D5.Port == GPIOD) ||
            (DISP_HandleStruct->D6.Port == GPIOD) || (DISP_HandleStruct->D7.Port == GPIOD) ||
            (DISP_HandleStruct->RS.Port == GPIOD) || (DISP_HandleStruct->E.Port == GPIOD)) {
        __GPIOD_CLK_ENABLE();
    }
#endif
#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F070xB) || \
    defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F030xC)
    if ((DISP_HandleStruct->D4.Port == GPIOE) || (DISP_HandleStruct->D5.Port == GPIOE) ||
            (DISP_HandleStruct->D6.Port == GPIOE) || (DISP_HandleStruct->D7.Port == GPIOE) ||
            (DISP_HandleStruct->RS.Port == GPIOE) || (DISP_HandleStruct->E.Port == GPIOE)) {
        __GPIOE_CLK_ENABLE();
    }
#endif
    if ((DISP_HandleStruct->D4.Port == GPIOF) || (DISP_HandleStruct->D5.Port == GPIOF) ||
            (DISP_HandleStruct->D6.Port == GPIOF) || (DISP_HandleStruct->D7.Port == GPIOF) ||
            (DISP_HandleStruct->RS.Port == GPIOF) || (DISP_HandleStruct->E.Port == GPIOF)) {
        __GPIOF_CLK_ENABLE();
    }
}

void DISP_CMD(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t cmd)
{
    DISP_Write(DISP_HandleStruct, cmd, 0);
}

void DISP_Puts(DISP_HandleTypeDef *DISP_HandleStruct, char *data)
{
    while (*data != '\0') {
        DISP_Putc(DISP_HandleStruct, *data++);
    }
}

void DISP_Putc(DISP_HandleTypeDef *DISP_HandleStruct, char c)
{
    DISP_Write(DISP_HandleStruct, c, 1);
}

void DISP_Out(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data)
{
    HAL_GPIO_WritePin(DISP_HandleStruct->D7.Port, DISP_HandleStruct->D7.Pin, data & (1 << 7));
    HAL_GPIO_WritePin(DISP_HandleStruct->D6.Port, DISP_HandleStruct->D6.Pin, data & (1 << 6));
    HAL_GPIO_WritePin(DISP_HandleStruct->D5.Port, DISP_HandleStruct->D5.Pin, data & (1 << 5));
    HAL_GPIO_WritePin(DISP_HandleStruct->D4.Port, DISP_HandleStruct->D4.Pin, data & (1 << 4));
}

//WARNING: If you don't have 8MHz as your clock frequency, change the Delay values
void DISP_Write(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data, uint8_t rs)
{
    HAL_GPIO_WritePin(DISP_HandleStruct->RS.Port, DISP_HandleStruct->RS.Pin, rs);

    DISP_Out(DISP_HandleStruct, data);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    DISP_Delay(50);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);

    DISP_Out(DISP_HandleStruct, (data << 4));
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    DISP_Delay(50);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    DISP_Delay(20);

    DISP_Out(DISP_HandleStruct, 0xff);
}

void DISP_Setcursor(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t x, uint8_t y)
{
    uint8_t data;

    switch (y) {
    case 1: // 1. Line
        data = DISP_DDRAM + DISP_DDADR_LINE1 + x;
        break;

    case 2: // 2. Line
        data = DISP_DDRAM + DISP_DDADR_LINE2 + x;
        break;

    case 3: // 3. Line
        data = DISP_DDRAM + DISP_DDADR_LINE3 + x;
        break;

    case 4: // 4. Line
        data = DISP_DDRAM + DISP_DDADR_LINE4 + x;
        break;

    default:
        return;
    }

    DISP_CMD(DISP_HandleStruct, data);
}

void DISP_Delay(uint16_t t)
{
    volatile uint16_t i;
    for (i = 0; i <= t; i++);
}
