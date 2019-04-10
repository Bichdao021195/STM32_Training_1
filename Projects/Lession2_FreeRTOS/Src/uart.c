#include <string.h>

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"

#include "uart.h"
#include "error.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef uart1Handle;

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}




int32_t uart1_init(void){
  uart1Handle.Instance        = USARTx;
  uart1Handle.Init.BaudRate   = 115200;
  uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart1Handle.Init.StopBits   = UART_STOPBITS_1;
  uart1Handle.Init.Parity     = UART_PARITY_NONE;
  uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  uart1Handle.Init.Mode       = UART_MODE_TX_RX;
  uart1Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
  if(HAL_UART_DeInit(&uart1Handle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&uart1Handle) != HAL_OK)
  {
    Error_Handler();
  }
    return 0;
};
int32_t uart1_deinit(void){
    return 0;
};

/**
  * @brief  Tx Transfer completed callback
  * @param  uart1Handle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uart1Handle){
}

/**
  * @brief  Rx Transfer completed callback
  * @param  uart1Handle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart1Handle){
}

/**
  * @brief  UART error callbacks
  * @param  uart1Handle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *uart1Handle)
{
    Error_Handler();
}

/* Private functions ---------------------------------------------------------*/
int fputc(int ch, FILE * p_file)
{
    //uart1_send_char((char)(ch & 0xff));
  HAL_UART_Transmit(&uart1Handle,(uint8_t *)&ch,1,0xffff);  
	return ch;
}		

int32_t uart1_send_char(char data){
    HAL_StatusTypeDef ret;
	  ret = HAL_UART_Transmit(&uart1Handle,(uint8_t *)&data,1,0xffff);
    if(ret != HAL_OK){
        return -1;
		} else {
			  return 0;
    }			
}
int32_t uart1_send_string(const char * in){
    HAL_StatusTypeDef ret;
	  ret = HAL_UART_Transmit(&uart1Handle,(uint8_t *)in,strlen(in),0xffff);
    if(ret != HAL_OK){
        return -1;
		} else {
			  return strlen(in);
    }		
	  
}

