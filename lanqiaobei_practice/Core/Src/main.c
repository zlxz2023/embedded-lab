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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdint.h"
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
uint8_t B1_State, B1_Last_State;
uint8_t B2_State, B2_Last_State;
uint8_t B3_State, B3_Last_State;
uint8_t B4_State, B4_Last_State;
uint8_t count, rec_flag, rec_data;  // 接收缓冲区的下标, 串口接收标志, 串口接收数据
char text[20], send_buff[255], rec_buff[255];
double vol_r37, vol_r38;  // R37的电压
uint8_t num_overflow, number;  // 数据是否大于10的标志， 数据
uint8_t open_count, led1_mode=1, led2_mode=1, tim2_flag, tim3_flag;  // 开机次数
uint16_t X=2, Y=2, T=0, fre_r39;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LED_Show(uint8_t led, uint8_t mode);
void KEY_Scan(void);
void LCD_Proc(void);
void USART_Proc(void);
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
  MX_TIM2_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
    // TIM2
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADC_Start(&hadc2);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start(&hadc1);
    // USART1
    HAL_UART_Receive_IT(&huart1, &rec_data, 1);
    // I2C
    I2CInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      // USART_Proc();
      // KEY_Scan();
      // LED_Proc();
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
void KEY_Scan(void){
    B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
    B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if(B1_State==0 && B1_Last_State==1){
        
    }
    if(B2_State==0 && B2_Last_State==1){
        
    }
    if(B3_State==0 && B3_Last_State==1){
        
    }
    if(B4_State==0 && B4_Last_State==1){
        
    }
    B1_Last_State=B1_State;
    B2_Last_State=B2_State;
    B3_Last_State=B3_State;
    B4_Last_State=B4_State;
}
double Get_Vol(ADC_HandleTypeDef* hadc){
    HAL_ADC_Start(hadc);
    uint32_t cnt=0;
    cnt=HAL_ADC_GetValue(hadc);
    return cnt*3.3/4096;
}
void LCD_Proc(void){
    sprintf(text, "        DATA        ");
    LCD_DisplayStringLine(Line3, (uint8_t*)text);
//    vol_r37=Get_Vol(&hadc2);
//    sprintf(text, "     VR37:%.2fV     ", vol_r37);
//    LCD_DisplayStringLine(Line4, (uint8_t*)text);
    sprintf(text, "     PR39:%4dHz", fre_r39);
    LCD_DisplayStringLine(Line4, (uint8_t*)text);
//    vol_r38=Get_Vol(&hadc1);
//    sprintf(text, "     VR38:%.2fV     ", vol_r38);
//    LCD_DisplayStringLine(Line5, (uint8_t*)text);
//    sprintf(text, "        DATA        ");
//    LCD_DisplayStringLine(Line2, (uint8_t*)text);
//    sprintf(text, "        X:%d        ", X);
//    LCD_DisplayStringLine(Line3, (uint8_t*)text);
//    sprintf(text, "        Y:%d        ", Y);
//    LCD_DisplayStringLine(Line4, (uint8_t*)text);
//    T = X*Y;
//    sprintf(text, "        T:%d    ", T);
//    LCD_DisplayStringLine(Line5, (uint8_t*)text);
}
void LED_Proc(void){
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
    if(vol_r37>2.4){
        __HAL_TIM_SET_AUTORELOAD(&htim2, 100-1);
    }else{
        __HAL_TIM_SET_AUTORELOAD(&htim2, 1000/6-1);
    }
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
void USART_Proc(void){
    if(rec_flag){
        rec_flag=0;
        LCD_Clear(Black);
        if(strcmp("X", rec_buff)==0){
            
        }else if(strcmp("Y", rec_buff)==0){
            
        }else{
            
        }
        memset(rec_buff, 0, 255);
        count=0;
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        rec_flag=1;
        rec_buff[count++]=rec_data;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        led1_mode^=1;
    }
    if(htim->Instance==TIM3){
        led2_mode^=1;
    }
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM16){
        uint32_t temp=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        TIM16->CNT=0;
        fre_r39=80000000/(80*temp);
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
