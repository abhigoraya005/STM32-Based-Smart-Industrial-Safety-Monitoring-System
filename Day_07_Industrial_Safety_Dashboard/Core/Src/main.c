#include "main.h"
#include <stdio.h>
#include <string.h>

/* UART Handle */
UART_HandleTypeDef huart1;

/* ADC Handle */
ADC_HandleTypeDef hadc1;

/* Variables */
uint32_t gas_value = 0;
uint32_t temp_value = 0;
uint32_t distance_value = 0;

char uart_buffer[200];

/* Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);

uint32_t Read_ADC(uint32_t channel);

void Send_Dashboard(void);
void Check_Alerts(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_USART1_UART_Init();

    MX_ADC1_Init();

    while (1)
    {
        /* Read Sensors */
        gas_value = Read_ADC(ADC_CHANNEL_1);

        temp_value = Read_ADC(ADC_CHANNEL_2);

        distance_value = Read_ADC(ADC_CHANNEL_3);

        /* Convert ADC to Percentage */
        gas_value = (gas_value * 100) / 4095;

        temp_value = (temp_value * 100) / 4095;

        distance_value = (distance_value * 100) / 4095;

        /* Check Alerts */
        Check_Alerts();

        /* UART Dashboard */
        Send_Dashboard();

        HAL_Delay(1000);
    }
}

/* ADC READ FUNCTION */
uint32_t Read_ADC(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;

    sConfig.Rank = ADC_REGULAR_RANK_1;

    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&hadc1);
}

/* ALERT CHECK */
void Check_Alerts(void)
{
    /* RESET ALL */
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_12 |
                      GPIO_PIN_13 |
                      GPIO_PIN_14 |
                      GPIO_PIN_15,
                      GPIO_PIN_RESET);

    /* GAS ALERT */
    if(gas_value > 60)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_12,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_15,
                          GPIO_PIN_SET);
    }

    /* TEMP ALERT */
    if(temp_value > 70)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_13,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_15,
                          GPIO_PIN_SET);
    }

    /* INTRUDER ALERT */
    if(distance_value < 30)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_14,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_15,
                          GPIO_PIN_SET);
    }
}

/* UART DASHBOARD */
void Send_Dashboard(void)
{
    sprintf(uart_buffer,

"\r\n===== INDUSTRIAL SAFETY DASHBOARD =====\r\n"
"Gas Level    : %lu %%\r\n"
"Temperature  : %lu C\r\n"
"Distance     : %lu cm\r\n\r\n"

"STATUS:\r\n",

gas_value,
temp_value,
distance_value);

    HAL_UART_Transmit(&huart1,
                      (uint8_t*)uart_buffer,
                      strlen(uart_buffer),
                      HAL_MAX_DELAY);

    /* GAS */
    if(gas_value > 60)
    {
        sprintf(uart_buffer,
                "[!] GAS LEAK DETECTED\r\n");

        HAL_UART_Transmit(&huart1,
                          (uint8_t*)uart_buffer,
                          strlen(uart_buffer),
                          HAL_MAX_DELAY);
    }

    /* TEMP */
    if(temp_value > 70)
    {
        sprintf(uart_buffer,
                "[!] HIGH TEMPERATURE\r\n");

        HAL_UART_Transmit(&huart1,
                          (uint8_t*)uart_buffer,
                          strlen(uart_buffer),
                          HAL_MAX_DELAY);
    }

    /* DISTANCE */
    if(distance_value < 30)
    {
        sprintf(uart_buffer,
                "[!] INTRUDER ALERT\r\n");

        HAL_UART_Transmit(&huart1,
                          (uint8_t*)uart_buffer,
                          strlen(uart_buffer),
                          HAL_MAX_DELAY);
    }

    sprintf(uart_buffer,
            "=======================================\r\n");

    HAL_UART_Transmit(&huart1,
                      (uint8_t*)uart_buffer,
                      strlen(uart_buffer),
                      HAL_MAX_DELAY);
}

/* ADC INIT */
static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;

    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;

    hadc1.Init.ContinuousConvMode = DISABLE;

    hadc1.Init.DiscontinuousConvMode = DISABLE;

    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;

    hadc1.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_1;

    sConfig.Rank = ADC_REGULAR_RANK_1;

    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/* UART INIT */
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

    HAL_UART_Init(&huart1);
}

/* GPIO INIT */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_12 |
                      GPIO_PIN_13 |
                      GPIO_PIN_14 |
                      GPIO_PIN_15,
                      GPIO_PIN_RESET);

    GPIO_InitStruct.Pin =
            GPIO_PIN_12 |
            GPIO_PIN_13 |
            GPIO_PIN_14 |
            GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* CLOCK */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState = RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK |
            RCC_CLOCKTYPE_SYSCLK |
            RCC_CLOCKTYPE_PCLK1 |
            RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                        FLASH_LATENCY_0);
}

/* ERROR */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
