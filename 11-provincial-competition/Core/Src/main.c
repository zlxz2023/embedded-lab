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
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "lcd.h"
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
char* mode[2]={"MANU", "AUTO"};uint8_t auto_flag=1, manu_para;  // 模式数组, 自动模式标志, 是否进入参数配置
double VR37;  // R37的电压值
uint8_t B1_State, B1_Last_State;
uint8_t B2_State, B2_Last_State;
uint8_t B3_State, B3_Last_State;
uint8_t B4_State, B4_Last_State;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void LED_Proc(void);
void LED_Show(uint8_t led, uint8_t led_mode);
double Get_ADC_Value(void);
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
  MX_ADC2_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    // PWM
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);  // PA6
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);  // PA7
    TIM16->CCR1=10;
    TIM17->CCR1=10;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      VR37=Get_ADC_Value();
      LCD_Proc();
      if(auto_flag){
          uint16_t temp=(uint16_t)((VR37/3.3)*100);
          TIM16->CCR1=temp;
          TIM17->CCR1=temp;
      }
      KEY_Proc();
      LED_Proc();
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
double Get_ADC_Value(void){
    HAL_ADC_Start(&hadc2);
    uint32_t temp=HAL_ADC_GetValue(&hadc2);
    return temp*3.3/4096;
}
void LCD_Proc(void){
    if(manu_para==0){
        sprintf(text, "      Data          ");
        LCD_DisplayStringLine(Line0, (uint8_t*)text);
        sprintf(text, "    V:%.2fV         ", VR37);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "    Mode:%-4s       ", mode[auto_flag]);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }
    if(auto_flag==0 && manu_para==1){
        sprintf(text, "      Para          ");
        LCD_DisplayStringLine(Line0, (uint8_t*)text);
        sprintf(text, "    PA6:%02d%%         ", TIM16->CCR1);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "    PA7:%02d%%         ", TIM17->CCR1);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }
}
void LED_Show(uint8_t led, uint8_t led_mode){
    if(led_mode){
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
    if(auto_flag){
        LED_Show(0x01, 1);
    }else{
        LED_Show(0x01, 0);
    }
    if(manu_para==0){
        LED_Show(0x02, 1);
    }else{
        LED_Show(0x02, 0);
    }
}
void KEY_Proc(void){
    B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
    B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if(B1_State==0 && B1_Last_State==1){
        if(auto_flag==0){
            manu_para++;
            manu_para%=2;
            LCD_Clear(Black);
        }
    }
    if(B2_State==0 && B2_Last_State==1){
        if(auto_flag==0 && manu_para==1){
            TIM16->CCR1+=10;
            if(TIM16->CCR1>90) TIM16->CCR1=10;
        }
    }
    if(B3_State==0 && B3_Last_State==1){
        if(auto_flag==0 && manu_para==1){
            TIM17->CCR1+=10;
            if(TIM17->CCR1>90) TIM17->CCR1=10;
        }
    }
    if(B4_State==0 && B4_Last_State==1){
        auto_flag^=1;
        LCD_Clear(Black);
    }
    B1_Last_State=B1_State;
    B2_Last_State=B2_State;
    B3_Last_State=B3_State;
    B4_Last_State=B4_State;
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
