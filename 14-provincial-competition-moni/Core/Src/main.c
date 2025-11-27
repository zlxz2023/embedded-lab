/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "i2c_hal.h"
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
char text[20];
double V37, V38, res, V37_active=-1, V38_active=-1;
uint8_t page=0, mode=0, r37_flag, r38_flag;
uint8_t key_val,key_up,key_down,key_last_val;
uint8_t led1_flag, led2_flag, led3_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void LED_Proc(void);
void KEY_Proc(void);
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    // TIM
    TIM2->CNT=0;
    // I2C
    I2CInit();
    mode=EEPROM_Read(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      LED_Proc();
      KEY_Proc();
      if(r37_flag&&r38_flag){
          if(mode==0) res=V37_active-V38_active;
          if(mode==1) res=V37_active+V38_active;
          if(res>6.0){
              HAL_TIM_Base_Start_IT(&htim2);
          }else{
              HAL_TIM_Base_Stop_IT(&htim2);
              TIM2->CNT=0;
              led3_flag=0;
          }
      }
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
double Get_Vol(ADC_HandleTypeDef* hadc){
    HAL_ADC_Start(hadc);
    uint32_t temp = HAL_ADC_GetValue(hadc);
    return (temp*3.3)/4096;
}
void LCD_Proc(void){
    V37=Get_Vol(&hadc2);
    V38=Get_Vol(&hadc1);
    if(page==0){
        led1_flag=1;
        led2_flag=0;
        sprintf(text, "        DATA        ");
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "      R37:%.2fV     ", V37);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "      R38:%.2fV     ", V38);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }else{
        led1_flag=0;
        led2_flag=1;
        sprintf(text, "        COU         ");
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        if(r37_flag && r38_flag)
            sprintf(text, "      RES:%.2fV     ", res);
        else
            sprintf(text, "      RES:Error     ");
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "      MODE:%d        ", mode);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }
}
uint8_t KEY_Scan(void){
    uint8_t key_value=0;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_RESET){
        key_value=1;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==GPIO_PIN_RESET){
        key_value=2;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)==GPIO_PIN_RESET){
        key_value=3;
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_RESET){
        key_value=4;
    }
    return key_value;
}
void KEY_Proc(void){
    key_val=KEY_Scan();
    key_down=key_val&(key_val^key_last_val);
    key_up=~key_val&(key_val^key_last_val);
    key_last_val=key_val;
    if(key_down==1 && page==0){
        V37_active=V37;
        r37_flag=1;
    }
    if(key_down==2 && page==0){
        V38_active=V38;
        r38_flag=1;
    }
    if(key_down==3){
        page^=1;
    }
    if(key_down==4 && page==1){
        mode^=1;
        EEPROM_Write(0, mode);
    }
}
void LED_Show(uint8_t led, uint8_t mode){
    if(mode){
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}
void LED_Proc(void){
    LED_Show(0x01, led1_flag);
    LED_Show(0x02, led2_flag);
    LED_Show(0x04, led3_flag);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        led3_flag^=1;
    }
}
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
