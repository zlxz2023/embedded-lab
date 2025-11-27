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
char input[4] = {'*', '0', '1', '2'}, pwd[3];
uint8_t pd1, pd2, pd3, pd_index, mpd_index, mpd1, mpd2, mpd3, page, sec_3, tim_flag;
double r37;
uint8_t key_val, key_up, key_down, key_last_val;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LCD_Proc(void);
void KEY_Proc(void);
void PWD_Proc(void);
void LED_Proc(void);
void QIEHUAN(void);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    TIM2->CNT=0;
    // I2C
    I2CInit();
    pwd[0] = EEPROM_Read(0)+'0';
    pwd[1] = EEPROM_Read(1)+'0';
    pwd[2] = EEPROM_Read(2)+'0';
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      KEY_Proc();
      PWD_Proc();
      LCD_Proc();
      LED_Proc();
      QIEHUAN();
      if(page==1 && tim_flag==0){
          __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
          HAL_TIM_Base_Start_IT(&htim2);
          tim_flag=1;
      }
      if(mpd_index!=0){
          HAL_TIM_Base_Stop_IT(&htim2);
          tim_flag=0;
          TIM2->CNT=0;
      }
      if(sec_3==3){
          HAL_TIM_Base_Stop_IT(&htim2);
          page=0;
          tim_flag=0;
          TIM2->CNT=0;
          sec_3=0;
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
void Get_Vol(){
    HAL_ADC_Start(&hadc2);
    uint32_t temp=HAL_ADC_GetValue(&hadc2);
    r37=(3.3*temp)/4096;
}
void QIEHUAN(void){
    if(pd_index==0){
        if(r37<1.5) pd1=1;
        else if(r37>2.5) pd1=3;
        else pd1=2;
    }
    if(pd_index==1){
        if(r37<1.5) pd2=1;
        else if(r37>2.5) pd2=3;
        else pd2=2;
    }
    if(pd_index==2){
        if(r37<1.5) pd3=1;
        else if(r37>2.5) pd3=3;
        else pd3=2;
    }
    if(mpd_index==0){
        if(r37<1.5) mpd1=1;
        else if(r37>2.5) mpd1=3;
        else mpd1=2;
    }
    if(mpd_index==1){
        if(r37<1.5) mpd2=1;
        else if(r37>2.5) mpd2=3;
        else mpd2=2;
    }
    if(mpd_index==2){
        if(r37<1.5) mpd3=1;
        else if(r37>2.5) mpd3=3;
        else mpd3=2;
    }
}
void LCD_Proc(void){
    Get_Vol();
    if(page==0){
        sprintf(text, "        Lock        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "      Pass Word    ");
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "        %c %c %c       ", input[pd1], input[pd2], input[pd3]);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }else{
        sprintf(text, "        Set         ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "       Change       ");
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "       %c %c %c        ", input[mpd1], input[mpd2], input[mpd3]);
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
        if(page==0 && pd_index==0){
            pd_index++;
        }else if(page==0 && pd_index==1){
            pd_index++;
        }else if(page==0 && pd_index==2){
            pd_index++;
        }
        if(page==1){
            if(mpd_index==0){
                EEPROM_Write(0, input[mpd1]-'0');
                mpd_index++;
            }else if(mpd_index==1){
                EEPROM_Write(1, input[mpd2]-'0');
                mpd_index++;
            }else if(mpd_index==2){
                EEPROM_Write(2, input[mpd3]-'0');
                mpd_index++;
            }
        }
    }
}
void PWD_Proc(void){
    if(pd_index==3){
        pd_index=0;
        if(input[pd1]==pwd[0] && input[pd2]==pwd[1] && input[pd3]==pwd[2]){
            page=1;
        }
        pd1=pd2=pd3=0;
    }
    if(mpd_index==3){
        mpd_index=0;
        mpd1=mpd2=mpd3=0;
        page=0;
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
    }else{
        LED_Show(0x01, 0);
        LED_Show(0x02, 1);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        sec_3++;
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
