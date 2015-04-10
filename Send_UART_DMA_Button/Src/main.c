
#include "main.h"

GPIO_InitTypeDef GPIO_InitStruct;
UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;
__IO ITStatus UartReady = RESET;

static void SystemClock_Config(void);
HAL_StatusTypeDef UART_Send_String(UART_HandleTypeDef *huart, char *pData);
HAL_StatusTypeDef UART_Send_Char(UART_HandleTypeDef *huart, char pData);
HAL_StatusTypeDef UART_Send_Int(UART_HandleTypeDef *huart, int pData);
HAL_StatusTypeDef UART_Send_String_DMA(UART_HandleTypeDef *huart, char *pData);
uint8_t Char[4] = "ABCD";

int main(void)
{

    HAL_Init();
    SystemClock_Config();

    /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
    __GPIOA_CLK_ENABLE();
    /* -2- Configure PA3 in Interrupt Mode with Pull-up */
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* -3- Activate Interrupt */
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
    /* -4- Enable UART Clock */

    __USART1_CLK_ENABLE();

    /* -5- Configure TX/RX GPIO */

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* -6- Configure Uart*/
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    /* -7- Enable DMA clock */
    __DMA1_CLK_ENABLE();


    /* -8- Configure the DMA */
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance = DMA1_Channel2;
    hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode = DMA_NORMAL;
    hdma_tx.Init.Priority = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_tx);

    /* Associate the initialized DMA handle to the UART handle */
    __HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

    /* Configure the DMA handler for reception process */
    hdma_rx.Instance = DMA1_Channel3;
    hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode = DMA_NORMAL;
    hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

    /* -8- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);


    /* -9- Init the UART peripheral */
    HAL_UART_Init(&UartHandle);

    UART_Send_String_DMA(&UartHandle, "Amazing, I'm started up (: \r\n");
    while (1) {

    }
}

HAL_StatusTypeDef UART_Send_String(UART_HandleTypeDef *huart, char *pData)
{
    int length;
    for (length = 0; *(pData + length) != '\0'; length++);
    return HAL_UART_Transmit(huart, (uint8_t*) pData, length, 5000);
}

HAL_StatusTypeDef UART_Send_String_DMA(UART_HandleTypeDef *huart, char *pData)
{
    int length;
    for (length = 0; *(pData + length) != '\0'; length++);
    return HAL_UART_Transmit_DMA(huart, (uint8_t*) pData, length);
}

HAL_StatusTypeDef UART_Send_Char(UART_HandleTypeDef *huart, char pData)
{
    char *i;
    i = &pData;
    return HAL_UART_Transmit(huart, (uint8_t*) i, 1, 5000);
}

HAL_StatusTypeDef UART_Send_Int(UART_HandleTypeDef *huart, int pData)
{
    char SMesg[13];
    if (pData < 0) {
        SMesg[0] = '-';
        pData *= -1;
    } else {
        SMesg[0] = '+';
    }
    SMesg[1] = '0' + (pData / 1000000000); //billion
    SMesg[2] = '0' + ((pData / 100000000) % 10); //hundred million
    SMesg[3] = '0' + ((pData / 10000000) % 10); //ten million
    SMesg[4] = '0' + ((pData / 1000000) % 10); //million
    SMesg[5] = '0' + ((pData / 100000) % 10); //hundred thousand
    SMesg[6] = '0' + ((pData / 10000) % 10); //hundred thousand
    SMesg[7] = '0' + ((pData / 1000) % 10); //thousand
    SMesg[8] = '0' + ((pData / 100) % 10); //hundred
    SMesg[9] = '0' + ((pData / 10) % 10); //ten
    SMesg[10] = '0' + (pData % 10); //one
    SMesg[11] = '\r'; //"/r"	New Line
    SMesg[12] = '\n'; //"/n"
    return HAL_UART_Transmit(huart, (uint8_t*) SMesg, 13, 5000);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete*/
    UartReady = SET;
}

/**
 * @brief  Rx Transfer completed callback
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report end of DMA Rx transfer, and 
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete*/
    UartReady = SET;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_4) {
        while (UartReady != SET);
        UartReady = RESET;
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET){
            UART_Send_String_DMA(&UartHandle, "The button is pressed\r\n");
        } else {
            UART_Send_String_DMA(&UartHandle, "The button is released \r\n");
        }
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
