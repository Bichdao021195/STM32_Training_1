/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComIT/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "error.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComIT
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
TaskHandle_t vTask1Handle, vTask2Handle;
void *pvParameters ="Prinft Test";
/* Private define ------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void USER_init(void);
void vTask1 (void *pvParameter);
void vTask2 (void *pvParameter);
void vTaskFunction( void *pvParameters );
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
 
  HAL_Init();
	
  /* Configure the system clock to 216 MHz */
  SystemClock_Config();
	
	USER_init();
	xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL ,3, vTask1Handle);
	xTaskCreate(vTask2,"Task 2", configMINIMAL_STACK_SIZE, NULL,2,vTask2Handle);
	vTaskStartScheduler();
	for(;;);



}

void vTask1( void *pvParameters )
{
UBaseType_t uxPriority;
/* This task will always run before Task 2 as it is created with the higher
priority. Neither Task 1 nor Task 2 ever block so both will always be in
either the Running or the Ready state.
Query the priority at which this task is running - passing in NULL means
"return the calling task’s priority". */
uxPriority = uxTaskPriorityGet( NULL );
for( ;; )
{
/* Print out the name of this task. */
printf( "Task 1 is running\r\n" );
/* Setting the Task 2 priority above the Task 1 priority will cause
Task 2 to immediately start running (as then Task 2 will have the higher
priority of the two created tasks). Note the use of the handle to task
2 (xTask2Handle) in the call to vTaskPrioritySet(). Listing 35 shows how
the handle was obtained. */
printf( "About to raise the Task 2 priority\r\n" );
vTaskPrioritySet( vTask2Handle, ( uxPriority + 2 ) );
/* Task 1 will only run when it has a priority higher than Task 2.
Therefore, for this task to reach this point, Task 2 must already have
executed and set its priority back down to below the priority of this
task. */
}
}
void vTask2( void *pvParameters )
{
UBaseType_t uxPriority;
/* Task 1 will always run before this task as Task 1 is created with the
higher priority. Neither Task 1 nor Task 2 ever block so will always be
in either the Running or the Ready state.
Query the priority at which this task is running - passing in NULL means
"return the calling task’s priority". */
uxPriority = uxTaskPriorityGet( NULL );
for( ;; )
{
/* For this task to reach this point Task 1 must have already run and
set the priority of this task higher than its own.
Print out the name of this task. */
printf( "Task 2 is running\r\n" );
/* Set the priority of this task back down to its original value.
Passing in NULL as the task handle means "change the priority of the
calling task". Setting the priority below that of Task 1 will cause
Task 1 to immediately start running again – pre-empting this task. */
printf( "About to lower the Task 2 priority\r\n" );
vTaskPrioritySet( NULL, ( uxPriority - 2 ) );
}
}

void vTaskFunction( void *pvParameters )
{
char *pcTaskName;
TickType_t xLastWakeTime;
/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
pcTaskName = ( char * ) pvParameters;
/* The xLastWakeTime variable needs to be initialized with the current tick
count. Note that this is the only time the variable is written to explicitly.
After this xLastWakeTime is automatically updated within vTaskDelayUntil(). */
xLastWakeTime = xTaskGetTickCount();
/* As per most tasks, this task is implemented in an infinite loop. */
for( ;; )
{
/* Print out the name of this task. */
printf( pcTaskName );
/* This task should execute every 250 milliseconds exactly. As per
the vTaskDelay() function, time is measured in ticks, and the
pdMS_TO_TICKS() macro is used to convert milliseconds into ticks.
xLastWakeTime is automatically updated within vTaskDelayUntil(), so is not
explicitly updated by the task. */
vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 250 ) );
}
}

void USER_init(void){
	uart1_init();
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
