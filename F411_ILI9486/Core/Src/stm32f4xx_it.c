/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9486.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_LVL 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
unsigned short debounce = DEBOUNCE_LVL;
unsigned char is_button = 0x00;
unsigned char button_pressed = 0;
extern unsigned char Times_New_Roman12x12[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern TIM_HandleTypeDef htim10;
/* USER CODE BEGIN EV */
//extern uint16_t Timer1, Timer2;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim10);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */
  HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_RESET);
  if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = 'F';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //F1 button
  }
  else if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '1';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      ////is_button = 0x00;
      button_pressed = 0;
    }
    //1 button
  }
  else if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '4';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      ////is_button = 0x00;
      button_pressed = 0;
    }
    //4 button
  }
  else if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '7';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //7 button
  }
  else if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '<';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //< button
  }
  else if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_RESET);
  if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = 'G';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //F2 button
  }
  else if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '2';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //2 button
  }
  else if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '5';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //5 button
  }
  else if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '8';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //8 button
  }
  else if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '0';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //0 button
  }
  else if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  
  HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_RESET);
  if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '#';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //# button
  }
  else if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '3';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //3 button
  }
  else if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '6';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //6 button
  }
  else if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '9';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //9 button
  }
  else if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '>';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //> button
  }
  else if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  
  HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_SET);
  if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '*';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //* button
  }
  else if (HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = '^';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //^ button
  }
  else if (HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = 'v';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //v button
  }
  else if (HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = 's';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //s button
  }
  else if (HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  
  if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1)
  {
    if (debounce > 0)
    {
      debounce--;
    }
    if (debounce == 0)
    {
      is_button = 'e';
      button_pressed = 1;
      while(HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 1);
      debounce = DEBOUNCE_LVL;
      //is_button = 0x00;
      button_pressed = 0;
    }
    //e button
  }
  else if (HAL_GPIO_ReadPin(ROW_5_GPIO_Port, ROW_5_Pin) == 0)
  {
    debounce = DEBOUNCE_LVL;
  }
  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
