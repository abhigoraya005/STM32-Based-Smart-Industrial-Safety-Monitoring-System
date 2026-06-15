/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Smart Industrial Buzzer Alarm System
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

char uart_buffer[100];

uint8_t alarm_state = 0;

/* USER CODE END PV */

void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

void Activate_Alarm(void);

void Deactivate_Alarm(void);

void Send_UART_Message(char *message);

/* USER CODE END PFP */

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  MX_USART1_UART_Init();

  while (1)
  {
      Activate_Alarm();

      HAL_Delay(3000);

      Deactivate_Alarm();

      HAL_Delay(3000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                          FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;

  huart1.Init.BaudRate = 9600;

  huart1.Init.WordLength = UART_WORDLENGTH_8B;

  huart1.Init.StopBits = UART_STOPBITS_1;

  huart1.Init.Parity = UART_PARITY_NONE;

  huart1.Init.Mode = UART_MODE_TX_RX;

  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
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

  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB,
                    GPIO_PIN_12 | GPIO_PIN_13,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

void Activate_Alarm(void)
{
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_12,
                      GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_13,
                      GPIO_PIN_SET);

    Send_UART_Message("ALARM ACTIVATED\r\n");
}

void Deactivate_Alarm(void)
{
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_12,
                      GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_13,
                      GPIO_PIN_RESET);

    Send_UART_Message("SYSTEM NORMAL\r\n");
}

void Send_UART_Message(char *message)
{
    HAL_UART_Transmit(&huart1,
                      (uint8_t*)message,
                      strlen(message),
                      HAL_MAX_DELAY);
}

/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();

  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file,
                   uint32_t line)
{
}

#endif
