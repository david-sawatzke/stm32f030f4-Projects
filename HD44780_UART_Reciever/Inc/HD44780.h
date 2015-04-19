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

#define HD44780_CLR                 (1<<0)    // DB0: clear display
#define HD44780_RST                 (0x30)    // Reset Display
#define HD44780_HOME                (1<<1)    // DB1: return to home position

#define HD44780_ENTRY_MODE          (1<<2)    // DB2: set entry mode
#define HD44780_ENTRY_INC           (1<<1)    // DB1: 1=increment, 0=decrement
#define HD44780_ENTRY_SHIFT         (1<<0)    // DB0: 1=display shift on

#define HD44780_HD44780LAYMODE         (1<<3)    // DB3: turn lcd/cursor on
#define HD44780_HD44780LAYMODE_ON      (1<<2)    // DB2: turn display on
#define HD44780_HD44780LAYMODE_CURSOR  (1<<1)    // DB1: turn cursor on
#define HD44780_HD44780LAYMODE_BLINK   (1<<0)    // DB0: blinking cursor

#define HD44780_MOVE                (1<<4)    // DB4: move cursor/display
#define HD44780_MOVE_HD44780           (1<<3)    // DB3: move display (0-> cursor)
#define HD44780_MOVE_RIGHT          (1<<2)    // DB2: move right (0-> left)

#define HD44780_FUNCTION            (1<<5)    // DB5: function set
#define HD44780_FUNCTION_8BIT       (1<<4)    // DB4: set 8BIT mode (0->4BIT mode)
#define HD44780_FUNCTION_2LINES     (1<<3)    // DB3: two lines (0->one line)
#define HD44780_FUNCTION_10DOTS     (1<<2)    // DB2: 5x10 font (0->5x7 font)

#define HD44780_CGRAM               (1<<6)    // DB6: set CG RAM address
#define HD44780_DDRAM               (1<<7)    // DB7: set DD RAM address

#define HD44780_BUSY                (1<<7)    // DB7: HD44780 is busy

#define HD44780_DDADR_LINE1         0x00
#define HD44780_DDADR_LINE2         0x40
#define HD44780_DDADR_LINE3         0x10
#define HD44780_DDADR_LINE4         0x50

typedef struct {
    GPIO_TypeDef *Port;
    uint32_t Pin;
} HD44780_PinTypeDef;

typedef struct {
    HD44780_PinTypeDef D4;
    HD44780_PinTypeDef D5;
    HD44780_PinTypeDef D6;
    HD44780_PinTypeDef D7;
    HD44780_PinTypeDef RS;
    HD44780_PinTypeDef E;
    uint8_t Mode;
    uint8_t Lines;
} HD44780_HandleTypeDef;

void HD44780_GPIO_Clk_Init(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct);
HAL_StatusTypeDef HD44780_Init(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct);
void HD44780_Write(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t data, uint8_t rs);
void HD44780_CMD(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t cmd);
void HD44780_Putc(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, char c);
void HD44780_Out(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t data);
void HD44780_Setcursor(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, uint8_t x, uint8_t y);
void HD44780_Puts(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, char *data);
void HD44780_PutInt(HD44780_HandleTypeDef *HD44780_HandleFunctionStruct, int pData);
void HD44780_Delay(uint16_t t);



#ifdef	__cplusplus
}
#endif

#endif	/* HD44780_H */

