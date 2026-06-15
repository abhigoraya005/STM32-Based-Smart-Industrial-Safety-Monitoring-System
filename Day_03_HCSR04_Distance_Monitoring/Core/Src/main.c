/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : EV ADAS Distance Monitoring System
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

uint32_t pulse_time = 0;

float distance = 0;

char uart_buffer[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

void HCSR04_Trigger(void);

uint32_t HCSR04_Read(void);

void Send_UART_Data(void);

void Check_Distance_Warning(void);

/* USER CODE END PFP */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  MX_USART1_UART_Init();

  while (1)
  {
      HCSR04_Trigger();

      pulse_time = HCSR04_Read();

      distance = pulse_time;

      Check_Distance_Warning();

      Send_UART_Data();

      HAL_Delay(1000);
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

  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* PA0 LOW */
  HAL_GPIO_WritePin(GPIOA,
                    GPIO_PIN_0,
                    GPIO_PIN_RESET);

  /* PB12 LOW */
  HAL_GPIO_WritePin(GPIOB,
                    GPIO_PIN_12,
                    GPIO_PIN_RESET);

  /* PA0 OUTPUT */
  GPIO_InitStruct.Pin = GPIO_PIN_0;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PB12 OUTPUT */
  GPIO_InitStruct.Pin = GPIO_PIN_12;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

void HCSR04_Trigger(void)
{
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_0,
                      GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_0,
                      GPIO_PIN_RESET);
}

uint32_t HCSR04_Read(void)
{
    static uint32_t fake_distance = 5;

    fake_distance += 5;

    if(fake_distance > 50)
    {
        fake_distance = 5;
    }

    return fake_distance;
}

void Send_UART_Data(void)
{
    sprintf(uart_buffer,
            "Distance: %.2f cm\r\n",
            distance);

    HAL_UART_Transmit(&huart1,
                      (uint8_t*)uart_buffer,
                      strlen(uart_buffer),
                      HAL_MAX_DELAY);
}

void Check_Distance_Warning(void)
{
    if(distance < 20.0f)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_12,
                          GPIO_PIN_SET);

        sprintf(uart_buffer,
                "WARNING: OBJECT TOO CLOSE\r\n");

        HAL_UART_Transmit(&huart1,
                          (uint8_t*)uart_buffer,
                          strlen(uart_buffer),
                          HAL_MAX_DELAY);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_12,
                          GPIO_PIN_RESET);
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
