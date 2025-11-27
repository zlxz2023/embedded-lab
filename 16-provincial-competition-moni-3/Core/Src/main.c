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
uint8_t page, rate, high=100, low=60, line, max_rate=0, min_rate=222, con;
uint8_t key_val, key_down, key_up, key_last_val;
uint32_t r39;
double r37;uint8_t md_flag=0, temp_high, temp_low, quit, led_flag, sec_6, last_item=2;  // 表示上一个con++的情况
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void RATE_Proc(void);
void Rate(uint32_t f);
void Get_Vol(void);
void LED_Proc(void);
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
  MX_TIM3_Init();
  MX_ADC2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      Rate(r39);
      Get_Vol();
      KEY_Proc();
      if(r39>0) RATE_Proc();
      LCD_Proc();
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
void Get_Vol(){
    HAL_ADC_Start(&hadc2);
    uint32_t temp=HAL_ADC_GetValue(&hadc2);
    r37=(3.3*temp)/4096;
}
uint8_t thread(double v){
    if(v-1.0<0){
        return 60;
    }else if(v-3.0>0){
        return 150;
    }else{
        return 45*v+15;
    }
}
void Rate(uint32_t f){
    if(f<=1000){
        rate=30;
    }else if(f>=2000){
        rate=200;
    }else{
        rate=(uint8_t)(0.17*f-140);
    }
}
void LCD_Proc(void){
    if(page==0){
        if(md_flag){
            md_flag=0;
            quit=2;
        }
        sprintf(text, "       HEART        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "      Rate:%d      ", rate);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "       Con:%d      ", con);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }else if(page==1){
        if(md_flag){
            md_flag=0;
            quit=2;  // 不正常退出
        }
        sprintf(text, "       RECORD       ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "       Max:%d      ", max_rate);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "       Min:%d      ", min_rate);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        line=0;
    }else{
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "      High:%d      ", high);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "       Low:%d      ", low);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
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
    if(key_down==1){
        page++;
        page%=3;
    }
    if(key_down==2 && page==2){
        line^=1;
    }
    if(key_down==3 && page==2){
        if(md_flag==1 && key_down==3){
            quit=1;
        }
        md_flag^=1;
        if(md_flag){
            temp_high=high;
            temp_low=low;
        }
    }
    if(key_down==4 && page==0){
        con=0;
    }
}
void RATE_Proc(void){
    if(rate>max_rate){
        max_rate=rate;
    }
    if(rate<min_rate){
        min_rate=rate;
    }
    if(md_flag && page==2){
        if(line==0) high=thread(r37);
        if(line==1) low=thread(r37);
    }else{
        if(last_item==2){  // con没有++
            if(rate>high){
                last_item=1;
                con++;
                led_flag=1;
                sec_6=0;
                TIM4->CNT=0;
                HAL_TIM_Base_Start_IT(&htim4);
            }else if(rate<low){
                last_item=0;
                con++;
                led_flag=1;
                sec_6=0;
                TIM4->CNT=0;
                HAL_TIM_Base_Start_IT(&htim4);
            }
        }else if(last_item==1){  // 表示上一个是high
            if(rate<low){
                last_item=0;
                con++;
                led_flag=1;
                sec_6=0;
                TIM4->CNT=0;
                HAL_TIM_Base_Start_IT(&htim4);
            }
        }else if(last_item==0){  // 表示上一个是low
            if(rate>high){
                last_item=1;
                con++;
                led_flag=1;
                sec_6=0;
                TIM4->CNT=0;
                HAL_TIM_Base_Start_IT(&htim4);
            }
        }
    }
    if(quit==2){
        high=temp_high;
        low=temp_low;
        quit=0;
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
    if(page==0){
        LED_Show(0x01, 1);
        LED_Show(0x02, 0);
        LED_Show(0x04, 0);
    }else if(page==1){
        LED_Show(0x01, 0);
        LED_Show(0x02, 1);
        LED_Show(0x04, 0);
    }else{
        LED_Show(0x01, 0);
        LED_Show(0x02, 0);
        LED_Show(0x04, 1);
    }
    if(md_flag){
        if(line==0){
            LED_Show(0x08, 1);
            LED_Show(0x10, 0);
        }else{
            LED_Show(0x08, 0);
            LED_Show(0x10, 1);
        }
    }else{
        LED_Show(0x08, 0);
        LED_Show(0x10, 0);
    }
    LED_Show(0x20, led_flag);
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM3){
        uint32_t temp=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        TIM3->CNT=0;
        r39=1000000/temp;
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM4){
        sec_6++;
        if(sec_6==6){
            HAL_TIM_Base_Stop_IT(htim);
            htim->Instance->CNT=0;
            led_flag=0;
        }
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
