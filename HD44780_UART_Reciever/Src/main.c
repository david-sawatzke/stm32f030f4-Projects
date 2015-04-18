
#include "main.h"
#define PosY_MAX 9
#define PosX_MAX 39

GPIO_InitTypeDef GPIO_InitStruct;
UART_HandleTypeDef UartHandle;
DISP_HandleTypeDef DISP_HandleStruct;
char Msg [PosY_MAX + 1][PosX_MAX + 1];
uint8_t PosX = 0;
uint8_t PosY = 0;
void SystemClock_Config(void);
void PrintScreen(uint8_t PosY, uint8_t PosX);

int main(void)
{
    //Init System
    HAL_Init();
    SystemClock_Config();
    //Allow Systick Interrupt for Time Measurement to interrupt other interrupts
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    //Fill Array with spaces
    for (PosY = 0; PosY <= PosY_MAX; PosY++) {
        for (PosX = 0; PosX <= PosX_MAX; PosX++) {
            Msg[PosY][PosX] = ' ';
        }
    }
    PosY = 0;
    PosX = 0;
    //Init Display (Does clock enabeling and IO Handeling for me)
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
    //Enable CLock
    __GPIOA_CLK_ENABLE();
    __USART1_CLK_ENABLE();
    //Activate PA3 as RX pin
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //Enable UART in rx mode
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_RX;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_UART_Init(&UartHandle);
    //Splash screen, gets disabled once first character arrives
    DISP_Puts(&DISP_HandleStruct, "UART Terminal");
    HAL_UART_Receive(&UartHandle, (uint8_t *) & Msg[0], 1, 0xFFFFFFFF);
    DISP_CMD(&DISP_HandleStruct, DISP_DISPLAYMODE | DISP_DISPLAYMODE_ON | DISP_DISPLAYMODE_BLINK);
    while (1) {
        //If Return
        if (Msg[PosY][PosX] == '\r') {
            //Delete return character
            Msg[PosY][PosX] = ' ';
            //Goto the next line
            if (PosY == 0) {
                PosY = 1;
            } else {
                PosY = 0;
            }
            //Make sure next line is empty
            for (PosX = 0; PosX < 16; PosX++) {
                Msg[PosY][PosX] = ' ';
            }
            PosX = 0;
        } else if ((Msg[PosY][PosX] == 127) || (Msg[PosY][PosX] == '\b')) {
            //If Delete, remove delete char
            Msg[PosY][PosX] = ' ';
            //Goto previous line if neccessary
            if (PosX == 0) {
                //Decide which one
                if (PosY == 0) {
                    PosY = 1;
                } else {
                    PosY = 0;
                }
                //So we land on pos 15 when decrements 
                PosX = 16;
            }
            PosX--;
            //Delete Deleted Character
            Msg[PosY][PosX] = ' ';
        } else {
            PosX++;
            //Goto next line if full
            if (PosX >= 16) {
                if (PosY == 1) {
                    PosY = 0;
                } else {
                    PosY = 1;
                }
                //Clear Line
                for (PosX = 0; PosX < 16; PosX++) {
                    Msg[PosY][PosX] = ' ';
                }
                PosX = 0;
            }
        }
        //Output screen
        PrintScreen(0, 0);
        //Put coursor at right position
        DISP_Setcursor(&DISP_HandleStruct, PosX, PosY);
        //At the end so splashscreen would work
        HAL_UART_Receive(&UartHandle, (uint8_t *) & Msg[PosY][PosX], 1, 0xFFFFFFFF);
    }
    return 0;
}

void PrintScreen(uint8_t Y, uint8_t X)
{
    uint8_t i;
    uint8_t Y_2nd;
    DISP_Setcursor(&DISP_HandleStruct, 0, 0);
    //Fill first line
    for (i = 0; (i < 16)&&(i + X <= PosX_MAX); i++) {
        DISP_Putc(&DISP_HandleStruct, Msg[Y][X + i]);
    }
    DISP_Setcursor(&DISP_HandleStruct, 0, 1);
    //So Circular Buferr works
    if (Y >= PosY_MAX) {
        Y_2nd = 0;
    } else {
        Y_2nd = Y + 1;
    }
    //Fill 2nd line
    for (i = 0; (i < 16)&&(i + X <= PosX_MAX); i++) {
        DISP_Putc(&DISP_HandleStruct, Msg[Y_2nd][X + i]);
    }
}

void SystemClock_Config(void)
{
    //Nothing to see here, 8MHz is currently enough
}
