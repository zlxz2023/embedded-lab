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
char* status[3] = {"Upper", "Lower", "Normal"};  // 3个状态
uint8_t setting_flag=0, line;  // 是否进入设置页面
uint8_t B1_State, B1_Last_State;
uint8_t B2_State, B2_Last_State;
uint8_t B3_State, B3_Last_State;
uint8_t B4_State, B4_Last_State;
double vol, max_vol=2.4, min_vol=1.2;  // 存储电压值
uint8_t up_led=0, low_led=1;  // 存储相应的LED序号
char* led_choose[8] = {"LD1", "LD2", "LD3", "LD4", "LD5", "LD6", "LD7", "LD8"};
uint8_t led[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
uint8_t status_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void Status_judge(void);
double GET_ADC_Vol(ADC_HandleTypeDef *hadc);
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
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // ADC2IN15
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      KEY_Proc();
      Status_judge();
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
void LCD_Proc(){
    if(setting_flag==0){
        vol=GET_ADC_Vol(&hadc2);
        sprintf(text, "        Main        ");
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "  Volt: %.2fV        ", vol);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "  Status: %-6s    ", status[status_flag]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
    }else{
        sprintf(text, "      Setting       ");
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        if(line==0){
            LCD_SetBackColor(Green);
        }else{
            LCD_SetBackColor(Black);
        }
        sprintf(text, " Max Volt: %.1fV      ", max_vol);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        if(line==1){
            LCD_SetBackColor(Green);
        }else{
            LCD_SetBackColor(Black);
        }
        sprintf(text, " Min Volt: %.1fV      ", min_vol);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        if(line==2){
            LCD_SetBackColor(Green);
        }else{
            LCD_SetBackColor(Black);
        }
        sprintf(text, " Upper: %-3s         ", led_choose[up_led]);
        LCD_DisplayStringLine(Line7, (uint8_t*)text);
        if(line==3){
            LCD_SetBackColor(Green);
        }else{
            LCD_SetBackColor(Black);
        }
        sprintf(text, " Lower: %-3s         ", led_choose[low_led]);
        LCD_DisplayStringLine(Line9, (uint8_t*)text);
        LCD_SetBackColor(Black);
    }
}
void KEY_Proc(){
    B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
    B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if(B1_State==0 && B1_Last_State==1){
        setting_flag^=1;
        LCD_Clear(Black);
    }
    if(B2_State==0 && B2_Last_State==1 && setting_flag==1){
        line++;
        if(line>3) line=0;
    }
    if(B3_State==0 && B3_Last_State==1 && setting_flag==1){
        if(line==0){
            max_vol+=0.3;
            if(max_vol>3.3) max_vol=3.3;
        }
        if(line==1){
            min_vol+=0.3;
            if(min_vol>3.3) min_vol=3.3;
            if(min_vol>=max_vol) min_vol-=0.3;
        }
        if(line==2){
            up_led++;
            if(up_led>7) up_led=0;
            if(up_led==low_led) up_led++;
            if(up_led>7) up_led=0;
            LED_Show(0xff, 0);
        }
        if(line==3){
            low_led++;
            if(low_led>7) low_led=0;
            if(up_led==low_led) low_led++;
            if(low_led>7) low_led=0;
            LED_Show(0xff, 0);
        }
    }
    if(B4_State==0 && B4_Last_State==1 && setting_flag==1){
        if(line==0){
            max_vol-=0.3;
            if(max_vol<0) max_vol=0;
            if(min_vol>=max_vol) max_vol+=0.3;
        }
        if(line==1){
            min_vol-=0.3;
            if(min_vol<0) min_vol=0;
        }
        if(line==2){
            up_led--;
            if(up_led==255) up_led=7;
            if(up_led==low_led) up_led--;
            if(up_led==255) up_led=7;
            LED_Show(0xff, 0);
        }
        if(line==3){
            low_led--;
            if(low_led==255) low_led=7;
            if(up_led==low_led) low_led--;
            if(low_led==255) low_led=7;
            LED_Show(0xff, 0);
        }
    }
    B1_Last_State=B1_State;
    B2_Last_State=B2_State;
    B3_Last_State=B3_State;
    B4_Last_State=B4_State;
}
void Status_judge(){
    if(vol>max_vol){
        status_flag=0;
        LED_Show(led[up_led], 1);
    }
    if(vol<=max_vol && vol>=min_vol){
        status_flag=2;
        LED_Show(led[up_led], 0);
        LED_Show(led[low_led], 0);
    }
    if(vol<min_vol){
        status_flag=1;
        LED_Show(led[low_led], 1);
    }
}
double GET_ADC_Vol(ADC_HandleTypeDef *hadc){
    HAL_ADC_Start(hadc);
    uint32_t vol=HAL_ADC_GetValue(hadc);
    return vol*3.3/4096;
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
