/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "at4nb.h"
#include "lcd.h"
#include "string.h"
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
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern AT4NB_MSG msgs[7];
extern Uart2DmaRcvData uart2DmaRcvData;
extern MsgIndex currentIndex;
extern uint8_t retryTimesBack[];
int debug;
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
  MX_ADC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	Lcd_Init();
	HAL_Delay(500);
	Lcd_Clear(WHITE);
	printf("system init done\r\n");
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		


		while (uart2DmaRcvData.rcvFlag == 0) //如果没有接受到NB的数据, LCD屏实时显示当前的状态
		{
			static uint8_t firstTime = 1;
			switch (currentIndex)
			{
				case REBOOT:
				if (firstTime)
				{
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "NB booting...");
					firstTime = 0;
				}
				break;
				
				case GET_SIGNAL_STRENGTH:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "getting signal");
				Gui_DrawFont_GBK16(10, 26, BLUE, WHITE, "strength...");
				break;
				
				case GET_IP_ADDR:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "getting IP");
						Gui_DrawFont_GBK16(10, 26, BLUE, WHITE, "address...");
				break;
				
				case CREATE_TCP_SOCKET:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "creating ");
						Gui_DrawFont_GBK16(10, 26, BLUE, WHITE, "socket...");
				break;
				
				case CONNECT_SOCKET:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "connecting ");
						Gui_DrawFont_GBK16(10, 26, BLUE, WHITE, "socket...");
				break;
				
				case SEND_MSG:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "sensoring...");
				break;
				
				case GET_MSG:
					Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "getting");
				Gui_DrawFont_GBK16(10, 26, BLUE, WHITE, "message...");
			}
		}
		
		 //已获取NB的数据
		printf("rcved: %s\r\n", uart2DmaRcvData.rcvBuff);
		
		
		if (strstr(uart2DmaRcvData.rcvBuff, msgs[currentIndex].feedBack) != NULL)  //初步校验获取到的数据
		{
			if (currentIndex == GET_SIGNAL_STRENGTH)  //需要进一步校验信号质量
			{
				uint16_t rssi;
				uint8_t *p;
				p = strstr(uart2DmaRcvData.rcvBuff, "+CSQ:");
				if (p)
				{
					sscanf(p, "+CSQ:%d", &rssi);
					
					if (rssi > 0 && rssi < 99)  //校验成功
					{
						char buf[5];
						sprintf(buf, "%d", rssi);
						msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];
						currentIndex++;
						Lcd_Clear(WHITE);
						Gui_DrawFont_GBK16(0, 10, BLUE, WHITE, "signal strength:");
						Gui_DrawFont_GBK16(50, 26, GREEN, WHITE, buf);
						HAL_Delay(1000);
						Lcd_Clear(WHITE);
						
					}
					else goto tryAgain;
				}
			}
			else if (currentIndex == GET_IP_ADDR)  //进一步校验地址是否有效
			{
				
				uint8_t *p;
				
				p = strstr(uart2DmaRcvData.rcvBuff, "+CGPADDR:0,");  //找到首地址
				if (p)  //0后有逗号说明已获取地址
				{
					uint8_t a1, a2, a3, a4;
					char buf[15];
					
					sscanf(p, "+CGPADDR:0,%d.%d.%d.%d", &a1, &a2, &a3, &a4);
					
						sprintf(buf, "%d.%d.%d.%d", a1, a2, a3, a4);
						
						Lcd_Clear(WHITE);
						Gui_DrawFont_GBK16(0, 10, BLUE, WHITE, "IP address:");
						Gui_DrawFont_GBK16(10, 26, GREEN, WHITE, buf);
						HAL_Delay(1000);
					Lcd_Clear(WHITE);
						
					msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];
					currentIndex++;
		
				}
				else  goto tryAgain;
			}
			else if (currentIndex == SEND_MSG)
			{
			
				sensoring();
			}
			else if (currentIndex == GET_MSG)
			{
				uint8_t *p;
				
				p = strstr(uart2DmaRcvData.rcvBuff, msgs[currentIndex].feedBack);  //找到首地址
				if (p)  
				{
					int relayCnd;
					sscanf(p, "1,64.69.43.237,10244,2,BB%d", &relayCnd);
					if (relayCnd == 1)
					{
						HAL_GPIO_WritePin(GPIOB, relay_Pin, GPIO_PIN_RESET);
					}
					else  
					{
						HAL_GPIO_WritePin(GPIOB, relay_Pin, GPIO_PIN_SET);
					}
				}
				
				currentIndex--;
				
				Lcd_Clear(WHITE);
			}
			else  //校验成功
			{
				msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];  //还原当前消息重试次数
				currentIndex++;
				HAL_Delay(1000);
				Lcd_Clear(WHITE);
			}
		}
		else  //校验数据失败
		{
			tryAgain:
			if (msgs[currentIndex].retryTimes > 0)  
			{
				msgs[currentIndex].retryTimes--;
				if (currentIndex != REBOOT)  
				{
					char buf[3];
					sprintf(buf, "%d", msgs[currentIndex].retryTimes);
					Gui_DrawFont_GBK16(50, 60, RED, WHITE, buf);
				}
			}
			else   //如果重试次数到，重启
			{
				msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];
				currentIndex = REBOOT;
			}
		}
		
		
		
		uart2DmaRcvData.rcvFlag = 0;
		HAL_UART_Transmit(&huart2, msgs[currentIndex].cmd, strlen(msgs[currentIndex].cmd), 100);
		
		
		
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  while(1) 
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
