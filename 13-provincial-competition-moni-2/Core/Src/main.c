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
double VR37=0, VR38=0, VP1=2.4, VP2=2.4;
uint8_t setting_flag=0;
uint8_t key_val, key_up, key_down, key_last_val;
uint32_t fre=1, capture_val;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void LED_Proc(void);
void KEY_Proc(void);
void LED_Show(uint8_t led, uint8_t mode);
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
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&htim17, TIM_CHANNEL_1);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      TIM17->CCR1 = 100;  // 写入任意值
      TIM17->SR |= TIM_SR_CC1IF;  // 手动置位中断标志
      HAL_TIM_IRQHandler(&htim17);  // 调用中断处理函数
      LCD_Proc();
      LED_Proc();
      KEY_Proc();
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
double Get_ADC_Vol(ADC_HandleTypeDef* hadc){
    HAL_ADC_Start(hadc);
    uint32_t temp = HAL_ADC_GetValue(hadc);
    return (temp*3.3/4096);
}
void LCD_Proc(void){
    VR37=Get_ADC_Vol(&hadc2);
    VR38=Get_ADC_Vol(&hadc1);
    if(setting_flag){
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "     VP1:%.1fV       ", VP1);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     VP2:%.1fV       ", VP2);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }else{
        sprintf(text, "        DATA        ");
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "     VR37:%.2fV     ", VR37);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     VR38:%.2fV     ", VR38);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }
    sprintf(text, "Freq: %d", fre);
    LCD_DisplayStringLine(Line8, (uint8_t*)text);
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
    VR37>VP1? LED_Show(0x01, 1):LED_Show(0x01, 0);
    VR38>VP2? LED_Show(0x02, 1):LED_Show(0x02, 0);
//    if(VR37<=VR38){
//        __HAL_TIM_SetAutoreload(&htim2, 5000-1);
//    }else{
//        __HAL_TIM_SetAutoreload(&htim2, 100-1);
//    }
}
uint8_t KEY_Scan(){
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
    if(key_down==1){
        setting_flag^=1;
    }
    if(key_down==2 && setting_flag){
        VP1+=0.3;
        if(VP1>3.3) VP1=0;
    }
    if(key_down==3 && setting_flag){
        VP2+=0.3;
        if(VP2>3.3) VP2=0;
    }
    key_last_val=key_val;
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    fre^=1;
    if(htim->Instance==TIM17){
//        capture_val=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 捕获到上升沿时，将CNT赋值给CCR
//        TIM17->CNT=0;
//        fre=80000000/80/capture_val;
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
