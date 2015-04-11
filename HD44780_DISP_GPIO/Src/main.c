
#include "main.h"



#define DISP_CLR                 (1<<0)    // DB0: clear display
#define DISP_RST                 (0x30)    // Reset Display
#define DISP_HOME                (1<<1)    // DB1: return to home position

#define DISP_ENTRY_MODE          (1<<2)    // DB2: set entry mode
#define DISP_ENTRY_INC           (1<<1)    // DB1: 1=increment, 0=decrement
#define DISP_ENTRY_SHIFT         (1<<0)    // DB0: 1=display shift on

#define DISP_DISPLAYMODE         (1<<3)    // DB3: turn lcd/cursor on
#define DISP_DISPLAYMODE_ON      (1<<2)    // DB2: turn display on
#define DISP_DISPLAYMODE_CURSOR  (1<<1)    // DB1: turn cursor on
#define DISP_DISPLAYMODE_BLINK   (1<<0)    // DB0: blinking cursor

#define DISP_MOVE                (1<<4)    // DB4: move cursor/display
#define DISP_MOVE_DISP           (1<<3)    // DB3: move display (0-> cursor)
#define DISP_MOVE_RIGHT          (1<<2)    // DB2: move right (0-> left)

#define DISP_FUNCTION            (1<<5)    // DB5: function set
#define DISP_FUNCTION_8BIT       (1<<4)    // DB4: set 8BIT mode (0->4BIT mode)
#define DISP_FUNCTION_2LINES     (1<<3)    // DB3: two lines (0->one line)
#define DISP_FUNCTION_10DOTS     (1<<2)    // DB2: 5x10 font (0->5x7 font)

#define DISP_CGRAM               (1<<6)    // DB6: set CG RAM address
#define DISP_DDRAM               (1<<7)    // DB7: set DD RAM address

#define DISP_BUSY                (1<<7)    // DB7: DISP is busy

typedef struct {
    GPIO_TypeDef *Port;
    uint32_t Pin;
} DISP_PinTypeDef;

typedef struct {
    DISP_PinTypeDef D4;
    DISP_PinTypeDef D5;
    DISP_PinTypeDef D6;
    DISP_PinTypeDef D7;
    DISP_PinTypeDef RS;
    DISP_PinTypeDef E;
    uint8_t Mode;
    uint8_t Lines;
} DISP_HandleTypeDef;
DISP_HandleTypeDef DISP_HandleStruct;

void DISP_GPIO_Clk_Init(DISP_HandleTypeDef *DISP_HandleStruct);
HAL_StatusTypeDef DISP_Init(DISP_HandleTypeDef *DISP_HandleStruct);
void DISP_Write(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data, uint8_t rs);
void DISP_CMD(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t cmd);
void DISP_Putc(DISP_HandleTypeDef *DISP_HandleStruct, char c);
void DISP_Out(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data);


void SystemClock_Config(void);

int main(void)
{

    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_Delay(2000);
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
    DISP_HandleStruct.E.Port = GPIOB;
    DISP_HandleStruct.E.Pin = GPIO_PIN_10;
    DISP_HandleStruct.Lines = 2;
    DISP_Init(&DISP_HandleStruct);
    DISP_Putc(&DISP_HandleStruct, 'a');
    while (1) {

    }
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
    DISP_Out(DISP_HandleStruct,0x00);
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

void DISP_Write(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data, uint8_t rs)
{
    HAL_GPIO_WritePin(DISP_HandleStruct->RS.Port, DISP_HandleStruct->RS.Pin, rs);

    DISP_Out(DISP_HandleStruct, data);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    DISP_Out(DISP_HandleStruct, (data << 4));
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DISP_HandleStruct->E.Port, DISP_HandleStruct->E.Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    DISP_Out(DISP_HandleStruct, 0xff);
    HAL_Delay(1);

}

void SystemClock_Config(void)
{

}
