/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
#include <stdbool.h>
#include "usbd_hid.h"
#include "keyboard.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
} keyboardHID;

keyboardHID keyboardhid = {0,0,0,0,0,0,0,0};

uint16_t kbd_sequence = 0;
uint8_t bit_num = 0;
uint8_t action_int = 11;
uint8_t parity_bit = 0;
uint16_t kbd_code = 0;
uint32_t time = 0;
uint8_t flag = 0;


struct queue_t keyq = {0};
struct keyboard_hid_t khid = {0};
uint32_t keyq_timeout = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern USBD_HandleTypeDef hUsbDeviceFS;
void kbd_arbeit()
{
  
  uint8_t bit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
  switch (action_int)
  {
      case 11:
        if (!bit)
        {
          action_int = 0;
        }
        break;
  case 0 ... 7:
    if (bit)
    {
      kbd_sequence |= (1<<action_int);
      parity_bit++;
    }
    else
    {
      kbd_sequence &=~(1<<action_int);
    }
    action_int++;
    break;
  case 8:
    if (bit)
    {
      if (parity_bit & (1<<0))
      {
        action_int = 11;
      }
      else
      {
        action_int = 9;
      }
    }
    else
    {
      if (parity_bit & (1<<0))
      {
        action_int = 9;
      }
      else
      {
        action_int = 11;
      }
    }
    break;
  case 9:
    if (bit)
    {
      kbd_code = kbd_sequence;
      /*if (kbd_code == 0xF0)
      {
        flag = 0x01;
      }*/
      action_int = 11;
      time = HAL_GetTick();
    }
    break;
  }
}

void transmit_mode(uint8_t mode)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  switch (mode)
  {
  case 1:
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIOB->BRR |= GPIO_PIN_4|GPIO_PIN_5;
    parity_bit = 0;
    action_int = 0;
    GPIOB->BSRR |= GPIO_PIN_5;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    break;
  case 2:
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /*Configure GPIO pin : PB5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void keyboard_data_output()
{
  switch (action_int)
  {
  case 0:
    transmit_mode(1);
    break;
  case 1 ... 8:
    if (kbd_code & (1<<action_int-1))
    {
      GPIOB->BSRR |= GPIO_BSRR_BS5;
      parity_bit++;
    }
    else
    {
      GPIOB->BSRR |= GPIO_BSRR_BR5;
    }
    break;
  case 9:
    if (parity_bit & (1<<0))
    {
      GPIOB->BSRR |= GPIO_BSRR_BR5;
    }
    else
    {
      GPIOB->BSRR |= GPIO_BSRR_BS5;
    }
    break;
  case 10:
    parity_bit = 0;
    kbd_code = 0;
    flag = 0;
    transmit_mode(2);
    break;
  }
  action_int++;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)               
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  queue_init(&keyq);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    handle_keys(&hUsbDeviceFS, &khid, &keyq, keyq_timeout/*, &hi2c2*/);
    /*if (HAL_GetTick() - time > 70)
    {
      kbd_code = 0xF0;
    }
    if (kbd_code == 0xF0)
    {
      keyboardhid.MODIFIER = 0x00;
      keyboardhid.KEYCODE1 = 0x00;
      keyboardhid.KEYCODE2 = 0x00;
      keyboardhid.KEYCODE3 = 0x00;
      keyboardhid.KEYCODE4 = 0x00;
      keyboardhid.KEYCODE5 = 0x00;
      keyboardhid.KEYCODE6 = 0x00;
      
    }
    if (kbd_code == 0x2C)
    {
      keyboardhid.KEYCODE1 = 0x17;
    }
    else if (kbd_code == 0x66)
    {
      keyboardhid.KEYCODE1 = 0x2A;
    }
    if (kbd_code == 0x12 || kbd_code == 0x59)
    {
      keyboardhid.MODIFIER = 0x02;
    }
    
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboardhid, sizeof (keyboardhid));
    */
    /*keyboardhid.MODIFIER = 0x02;  // left Shift
keyboardhid.KEYCODE1 = 0x04;  // press 'a'
keyboardhid.KEYCODE2 = 0x05;  // press 'b'
USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboardhid, sizeof (keyboardhid));
HAL_Delay (50);

keyboardhid.MODIFIER = 0x00;  // shift release
keyboardhid.KEYCODE1 = 0x00;  // release key
keyboardhid.KEYCODE2 = 0x00;  // release key
USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboardhid, sizeof (keyboardhid));
HAL_Delay (1000);*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
