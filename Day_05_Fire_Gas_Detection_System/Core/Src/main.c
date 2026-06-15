#include "main.h"
#include <stdio.h>
#include <string.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;

uint32_t adc_gas = 0;
uint32_t adc_temp = 0;

float gas_percent = 0;
float temperature = 0;

char uart_buffer[200];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);

void Read_Sensors(void);
void Send_UART_Data(void);
void Check_Warning(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_ADC1_Init();

    MX_USART1_UART_Init();

    while (1)
    {
        Read_Sensors();

        Check_Warning();

        Send_UART_Data();

        HAL_Delay(1000);
    }
}

void Read_Sensors(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /* GAS SENSOR PA0 */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    adc_gas = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    /* TEMPERATURE SENSOR PA1 */
    sConfig.Channel = ADC_CHANNEL_1;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    adc_temp = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    gas_percent = (adc_gas * 100.0f) / 4095.0f;

    temperature = ((adc_temp * 3.3f) / 4095.0f) * 100.0f;
}

void Check_Warning(void)
{
    if(gas_percent > 50 || temperature > 40)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}

void Send_UART_Data(void)
{
    sprintf(uart_buffer,
            "Gas: %.2f %% | Temp: %.2f C\r\n",
            gas_percent,
            temperature);

    HAL_UART_Transmit(&huart1,
                      (uint8_t*)uart_buffer,
                      strlen(uart_buffer),
                      HAL_MAX_DELAY);

    if(gas_percent > 50 || temperature > 40)
    {
        sprintf(uart_buffer,
                "!!! FIRE/GAS ALERT !!!\r\n");

        HAL_UART_Transmit(&huart1,
                          (uint8_t*)uart_buffer,
                          strlen(uart_buffer),
                          HAL_MAX_DELAY);
    }
}

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

    sConfig.Channel = ADC_CHANNEL_0;

    sConfig.Rank = ADC_REGULAR_RANK_1;

    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

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

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
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

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
