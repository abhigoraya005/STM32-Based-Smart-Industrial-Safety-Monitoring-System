#include "main.h"
#include <stdio.h>
#include <string.h>

/* HANDLES */
UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;

/* VARIABLES */
uint32_t gas_value = 0;
uint32_t temp_value = 0;
uint32_t distance_value = 0;

char uart_buffer[100];

/* LCD ADDRESS */
#define LCD_ADDR (0x27 << 1)

/* FUNCTION PROTOTYPES */
void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);

uint32_t Read_ADC(uint32_t channel);

void Check_Alerts(void);
void Send_UART_Data(void);

/* LCD FUNCTIONS */
void LCD_Send_Command(char cmd);
void LCD_Send_Data(char data);
void LCD_Init(void);
void LCD_Send_String(char *str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);

/* MAIN */
int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_USART1_UART_Init();

    MX_ADC1_Init();

    MX_I2C1_Init();

    LCD_Init();

    while (1)
    {
        /* READ ADC */
        gas_value = Read_ADC(ADC_CHANNEL_1);

        temp_value = Read_ADC(ADC_CHANNEL_2);

        distance_value = Read_ADC(ADC_CHANNEL_3);

        /* CONVERT TO PERCENTAGE */
        gas_value = (gas_value * 100) / 4095;

        temp_value = (temp_value * 100) / 4095;

        distance_value = (distance_value * 100) / 4095;

        /* CHECK ALERTS */
        Check_Alerts();

        /* UART */
        Send_UART_Data();

        /* LCD DISPLAY */
        LCD_Set_Cursor(1,1);

        sprintf(uart_buffer,
                "G:%lu T:%lu",
                gas_value,
                temp_value);

        LCD_Send_String(uart_buffer);

        LCD_Set_Cursor(2,1);

        sprintf(uart_buffer,
                "D:%lu",
                distance_value);

        LCD_Send_String(uart_buffer);

        HAL_Delay(1000);
    }
}

/* ADC READ */
uint32_t Read_ADC(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;

    sConfig.Rank = ADC_REGULAR_RANK_1;

    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1,
                              HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&hadc1);
}

/* ALERTS */
void Check_Alerts(void)
{
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_12 |
                      GPIO_PIN_13 |
                      GPIO_PIN_14 |
                      GPIO_PIN_15,
                      GPIO_PIN_RESET);

    /* GAS */
    if(gas_value > 60)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_12,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_15,
                          GPIO_PIN_SET);
    }

    /* TEMP */
    if(temp_value > 70)
    {
        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_13,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOB,
                          GPIO_PIN_15,
                          GPIO_PIN_SET);
    }

    /* DISTANCE */
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

/* UART */
void Send_UART_Data(void)
{
    sprintf(uart_buffer,

"\r\n===== INDUSTRIAL DASHBOARD =====\r\n"
"Gas : %lu %%\r\n"
"Temp: %lu C\r\n"
"Dist: %lu cm\r\n"
"================================\r\n",

gas_value,
temp_value,
distance_value);

    HAL_UART_Transmit(&huart1,
                      (uint8_t*)uart_buffer,
                      strlen(uart_buffer),
                      HAL_MAX_DELAY);
}

/* LCD LOW LEVEL */
void LCD_Send_Command(char cmd)
{
    HAL_I2C_Master_Transmit(&hi2c1,
                            LCD_ADDR,
                            (uint8_t*)&cmd,
                            1,
                            HAL_MAX_DELAY);
}

void LCD_Send_Data(char data)
{
    HAL_I2C_Master_Transmit(&hi2c1,
                            LCD_ADDR,
                            (uint8_t*)&data,
                            1,
                            HAL_MAX_DELAY);
}

void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Send_Command(0x38);

    LCD_Send_Command(0x0C);

    LCD_Send_Command(0x01);

    LCD_Send_Command(0x06);
}

void LCD_Send_String(char *str)
{
    while(*str)
    {
        LCD_Send_Data(*str++);
    }
}

void LCD_Set_Cursor(uint8_t row,
                    uint8_t col)
{
    uint8_t pos;

    if(row == 1)
    {
        pos = 0x80 + col - 1;
    }
    else
    {
        pos = 0xC0 + col - 1;
    }

    LCD_Send_Command(pos);
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

/* I2C INIT */
static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;

    hi2c1.Init.ClockSpeed = 100000;

    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;

    hi2c1.Init.OwnAddress1 = 0;

    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;

    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;

    hi2c1.Init.OwnAddress2 = 0;

    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;

    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c1);
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

    HAL_GPIO_Init(GPIOB,
                  &GPIO_InitStruct);
}

/* CLOCK */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType =
            RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
            RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
            RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
            RCC_PLL_NONE;

    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK |
            RCC_CLOCKTYPE_SYSCLK |
            RCC_CLOCKTYPE_PCLK1 |
            RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
            RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider =
            RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
            RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider =
            RCC_HCLK_DIV1;

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
