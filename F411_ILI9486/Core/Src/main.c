/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9486.h"
#include "fonts.h"
#include "user_diskio_spi.h"
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
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim10;

/* USER CODE BEGIN PV */
uint32_t size = 0;
/* SD Card variables*/
/*End of SD Card variables*/
extern unsigned char is_button;
extern unsigned char button_pressed;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim10);
  ILI9486_Init();
  ILI9486_Fill(0, 0, 479, 319, 255, 0, 0);
  
  FATFS FatFs; 	//Fatfs handle
  FIL fil; 		//File handle
  FRESULT fres;
  /**
  Modes for SD card FatFS:
  FA_READ
  FA_WRITE
  FA_OPEN_EXISTING
  FA_CREATE_NEW
  FA_CREATE_ALWAYS
  FA_OPEN_ALWAYS
  FA_OPEN_APPEND

  You can read more about that modes in http://microsin.net/programming/file-systems/fatfs-open.html
  **/
  fres = f_mount(&FatFs, "", 1); //Mount SD card file system
  if (fres != FR_OK)
  {
    ILI9486_WriteText(50, 50, "Can't mount SD card!!!", Times_New_Roman16x16, 16, 0xFFFF, (3<<6));
    HAL_Delay(500);
  }
  fres = f_open(&fil, "1.txt", FA_OPEN_ALWAYS|FA_WRITE);        //Init SD card and open file "1.txt" in RW mode
  if (fres != FR_OK)
  {
    ILI9486_WriteTestText(50,50, "Can't open file!!!",  (unsigned char*)&Font, 0xFFFF, (3<<6));
    HAL_Delay(500);
  }
  //fres = f_lseek(&fil, 0);
  uint32_t bytes = 0;
  fres = f_write(&fil, "It's test data, ignore that.", 28, (UINT*)bytes);
  f_sync(&fil);
  if (fres != FR_OK)
  {
    ILI9486_WriteTestText(50,50, "Can't write file!!!",  (unsigned char*)&Font, 0xFFFF, (3<<6));
    HAL_Delay(500);
  }
  f_close(&fil);
  f_mount(NULL, "", 0);
  HAL_Delay(500);
  f_mount(&FatFs, "", 1);
  f_open(&fil, "1.txt", FA_OPEN_ALWAYS|FA_READ);
  BYTE ReadBuff[100];   //Buffer for info from file
  TCHAR* rres = f_gets((TCHAR*)ReadBuff, 30, &fil);     //Reading info in buffer
  if (rres == 0)
  {
    ILI9486_WriteTestText(50,50, "Can't read file!!!",  (unsigned char*)&Font, 0xFFFF, (3<<6));
    HAL_Delay(500);
  }
  f_close(&fil);        //Close file. WARNING! Do not forget about this, if you don't close it, you will have an error in f_open!
  f_mount(NULL, "", 0); //Unmounting drive, needed for have opportunity to have an operation in SD card in other places in program
  ILI9486_Fill(0, 0, 479, 319, 255, 0, 0);
  uint8_t ilibuff[] = {0};
  sprintf(ilibuff, ReadBuff);
  ILI9486_WriteTestText(16,50, (char*)ilibuff, (unsigned char*)&Font, 0xFFFF, (3<<6));
  HAL_Delay(5000);      //Debug point, you can delete this
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //New value don't clear old, you can fix it or use it, for you want
    if (is_button != 0x00)      //is_button takes the value of pressed button from stm32f4xx_it.c file
    {
      if (is_button == 'F')
      {
        /**if (is_button_prev != is_button)
        {
          ILI9486_Fill(200, 200, 216, 232, 255, 0, 0); // If you want to clear write area, use this construction
          is_button_prev = is_button;
        }**/
        ILI9486_WriteTestText(200,200,"F1", (unsigned char*)&Font, 0xFFFF, (3<<0x06));
      }
      else if (is_button == 'G')
      {
        ILI9486_WriteTestText(200,200,"F2", (unsigned char*)&Font, 0xFFFF, (3<<0x06));
      }
      else if (is_button == 'v')
      {
        ILI9486_WriteTestText(200,200,"V", (unsigned char*)&Font, 0xFFFF, (3<<0x06));
      }
      else
      {
        ILI9486_WriteTestText(200,200, (char*)&is_button, (unsigned char*)&Font, 0xFFFF, (3<<0x06));
      }
    }
    //Display test's
    /**
    Display have next methods:
    void ILI9486_Fill(unsigned short xk1, unsigned short yk1, unsigned short xk2, unsigned short yk2, unsigned char r1, unsigned char g1, unsigned char b1)
    Filling area in x, y, j, z coords selected from RGB table colour (may worked is't correct, because that's func writes for 16-bit mode)

    void ILI9486_SetWindow(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1)
    Setting window for any operations, updating from left to right, upper to down

    void ILI9486_Init()
    Initialisation display, you can seen more in https://datasheetspdf.com/pdf-down/I/L/I/ILI9486-ILITEK.pdf

    void ILI9486_WriteTestText(uint16_t ix, uint16_t iy, char* iString, unsigned char* font, unsigned short font_color, unsigned short back_color)
    Writting text from x, y coords, from string, for selected font (font's in fonts.h), font size is constant from fonts.h, you must write it for evryone font, font color set's in HEX, from 0x00 to 0xFF, back color too.

    void ILI9486_DrawPixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B)
    Drawing pixel selected color, for you function's

    void ILI9486_DrawThickLine(unsigned short aXStart, unsigned short aYStart, unsigned short aXEnd, unsigned short aYEnd, unsigned char aThickness, unsigned char aThicknessMode, unsigned char R, unsigned char G, unsigned char B)
    Drawing thickness line, you can draw simple line if aThickness = 1, aThickness can't be < 1, a tThicknessMode is drawing mode, standard is LINE_THICKNESS_MIDDLE, you can use other modes from ili9486.h

    void ILI9486_DrawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char R, unsigned char G, unsigned char B, unsigned short thickness)
    Drawing rectangle, have bug with thickness, I recommended use this with thickness = 1, because if thickness > 1, displays not filled corner's

    void ILI9486_DrawCircle(unsigned int xc, unsigned int yc, unsigned int r, unsigned char R, unsigned char G, unsigned char B)
    Not tested function for drawing circle, maybe, worked, maybe, not, need testing.
    **/
    //ILI9486_DrawTestChar(200, 300, 'E', 0xFFFF, 0x0000);
    ILI9486_WriteTestText(50, 50, "Ñòðîêà", (unsigned char*)&Font, 0xFFFF, (3<<6));
    ILI9486_DrawCircle(50, 50, 20, 0, 255, 0);
    ILI9486_DrawThickLine(50, 100, 1, 2, 5, LINE_THICKNESS_MIDDLE, 255, 255, 255);
    /**
    Font's may be generated with errors, use GLCD Font Creator for create new font's (instruction on http://we.easyelectronics.ru/lcd_gfx/shrifty-s-glcd-font-creator-na-kolenke.html)
    **/
    ILI9486_WriteTestText(15,50,"ÀÁÂÃÄÅÆÇÈÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÜÚÝÞß", (unsigned char*)&Font, 0xFFFF, (3<<0x06));
    //ILI9486_WriteText(5,15,"àáâ", Verdana14x14rus, 14, 0xFFFF, (3<<0x06));
    ILI9486_WriteTestText(15,82,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", (unsigned char*)&Font, 0xFFFF, (3<<0x06));
    for (uint16_t x = 0; x < 465; x++)
    {
      ILI9486_WriteTestText(x, 305, "à", (unsigned char*)&Font, 0xFFFF, (31<<0x0B));
    }
    for (uint8_t i = 0; i < 255; i++)
    {
      ILI9486_Fill((HEIGHT/2)-25, (WIDTH/2)-25, (HEIGHT/2)+25, (WIDTH/2)+25, i, 0, 0);
    }
    for (uint8_t i = 0; i < 255; i++)
    {
      ILI9486_Fill((HEIGHT/2)-25, (WIDTH/2)-25, (HEIGHT/2)+25, (WIDTH/2)+25, 0, i, 0);
    }
    for (uint8_t i = 0; i < 255; i++)
    {
      ILI9486_Fill(459, 0, 470, 319, 0, 0, i);
    }
    ILI9486_DrawRect(50, 50, 100, 100, 0, 0, 255, 3);
    HAL_Delay(500);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 9999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 100;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D0_Pin|D1_Pin|D2_Pin|WR_Pin
                          |RS_Pin|D3_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|RST_Pin|CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin|COL_4_Pin|COL_3_Pin|COL_2_Pin
                          |COL_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ROW_1_Pin ROW_2_Pin ROW_3_Pin ROW_4_Pin
                           ROW_5_Pin */
  GPIO_InitStruct.Pin = ROW_1_Pin|ROW_2_Pin|ROW_3_Pin|ROW_4_Pin
                          |ROW_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin WR_Pin
                           RS_Pin D3_Pin D4_Pin D5_Pin
                           D6_Pin D7_Pin RST_Pin CS_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|WR_Pin
                          |RS_Pin|D3_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|RST_Pin|CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI_CS_Pin COL_4_Pin COL_3_Pin COL_2_Pin
                           COL_1_Pin */
  GPIO_InitStruct.Pin = SPI_CS_Pin|COL_4_Pin|COL_3_Pin|COL_2_Pin
                          |COL_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
