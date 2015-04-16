/* 
 * File:   HD44780.h
 * Author: dakesew
 *
 * Created on April 15, 2015, 5:16 PM
 */

#ifndef HD44780_H
#define	HD44780_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

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

#define DISP_DDADR_LINE1         0x00
#define DISP_DDADR_LINE2         0x40
#define DISP_DDADR_LINE3         0x10
#define DISP_DDADR_LINE4         0x50

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

void DISP_GPIO_Clk_Init(DISP_HandleTypeDef *DISP_HandleStruct);
HAL_StatusTypeDef DISP_Init(DISP_HandleTypeDef *DISP_HandleStruct);
void DISP_Write(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data, uint8_t rs);
void DISP_CMD(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t cmd);
void DISP_Putc(DISP_HandleTypeDef *DISP_HandleStruct, char c);
void DISP_Out(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t data);
void DISP_Setcursor(DISP_HandleTypeDef *DISP_HandleStruct, uint8_t x, uint8_t y);
void DISP_Puts(DISP_HandleTypeDef *DISP_HandleStruct, char *data);
void DISP_PutInt(DISP_HandleTypeDef *DISP_HandleStruct, int pData);
void DISP_Delay(uint16_t t);



#ifdef	__cplusplus
}
#endif

#endif	/* HD44780_H */

