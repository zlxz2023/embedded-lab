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
char text[20], rec_buff[50], send_buff[50];
uint8_t rec_data;
uint8_t hours=11, minutes=59, seconds=50;
uint8_t alarm_h=12, alarm_m=0, alarm_s=0;
uint8_t page, num[3], led_mode;  // 页码
uint8_t key_val, key_up, key_down, key_last_val;  // 按键
uint8_t select_1, select_2;  // 高亮
uint8_t modify, rtc_cnt, alarm_cnt, rtc_flag, alarm_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void LED_Proc(void);
void USART_Proc(void);
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
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM2--计时, TIM3--按键长短按, TIM4--LED闪烁
    __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start(&htim3);
    // RTC
    I2CInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      KEY_Proc();
      USART_Proc();
      if(page==0){
          __HAL_TIM_CLEAR_FLAG(&htim4,TIM_FLAG_UPDATE);
          HAL_TIM_Base_Start_IT(&htim4);
      }else{
          HAL_TIM_Base_Stop_IT(&htim4);
          TIM4->CNT=0;
          led_mode=0;
      }
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
void LCD_Proc(void){
    EEPROM_Read(3, num);
    rtc_cnt=num[0];
    alarm_cnt=num[1];
    if(page==0){
        sprintf(text, "        MAIN        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        LCD_DisplayChar(Line4,  (320 - (16 * 4)), 'R');
        LCD_DisplayChar(Line4,  (320 - (16 * 5)), 'T');
        LCD_DisplayChar(Line4,  (320 - (16 * 6)), 'C');
        LCD_DisplayChar(Line4,  (320 - (16 * 8)), hours/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 9)), hours%10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 11)), minutes/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 12)), minutes%10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 14)), seconds/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 15)), seconds%10+0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , ':');
    }
    if(page==1){
        sprintf(text, "     RTC-SETTING    ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        LCD_DisplayChar(Line4,  (320 - (16 * 4)), 'R');
        LCD_DisplayChar(Line4,  (320 - (16 * 5)), 'T');
        LCD_DisplayChar(Line4,  (320 - (16 * 6)), 'C');
        if(select_1==0){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 8)), hours/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 9)), hours%10+0x30);
        if(select_1==1){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 11)), minutes/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 12)), minutes%10+0x30);
        if(select_1==2){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 14)), seconds/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 15)), seconds%10+0x30);
        LCD_SetTextColor(White);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , ':');
    }
    if(page==2){
        if(alarm_cnt){
            EEPROM_Read(0, num);
            alarm_h=num[0];
            alarm_m=num[1];
            alarm_s=num[2];
        }
        sprintf(text, "    ALARM-SETTING   ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        LCD_DisplayChar(Line4,  (320 - (16 * 2)), 'A');
        LCD_DisplayChar(Line4,  (320 - (16 * 3)), 'l');
        LCD_DisplayChar(Line4,  (320 - (16 * 4)), 'a');
        LCD_DisplayChar(Line4,  (320 - (16 * 5)), 'r');
        LCD_DisplayChar(Line4,  (320 - (16 * 6)), 'm');
        if(select_2==0){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 8)), alarm_h/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 9)), alarm_h%10+0x30);
        if(select_2==1){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 11)), alarm_m/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 12)), alarm_m%10+0x30);
        if(select_2==2){
            LCD_SetTextColor(Red);
        }else{
            LCD_SetTextColor(White);
        }
        LCD_DisplayChar(Line4,  (320 - (16 * 14)), alarm_s/10+0x30);
        LCD_DisplayChar(Line4,  (320 - (16 * 15)), alarm_s%10+0x30);
        LCD_SetTextColor(White);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , ':');
    }
    sprintf(text, "      %02d-%02d-%05d", rtc_cnt, alarm_cnt, TIM3->CNT);
    LCD_DisplayStringLine(Line9, (uint8_t*)text);
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
    if(page==0){
        LED_Show(0x01, led_mode);
    }else{
        LED_Show(0x01, led_mode);
    }
    if(page==1){
        LED_Show(0x02, 1);
    }else{
        LED_Show(0x02, 0);
    }
    if(page==2){
        LED_Show(0x04, 1);
    }else{
        LED_Show(0x04, 0);
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
    if(key_down==1 && page!=2){
        page++;
        page%=2;
        if(page==1){
            HAL_TIM_Base_Stop_IT(&htim2);
            // TIM2->CNT=0;
        }
        if(page==0){
            __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
            HAL_TIM_Base_Start_IT(&htim2);
        }
        if(rtc_flag){
            rtc_cnt++;
            EEPROM_Write(3, rtc_cnt);
            rtc_flag=0;
        }
        LCD_Clear(Black);
    }
    if(key_down==1 && page==2){
        select_2++;
        select_2%=3;
    }
    if(key_down==2 && page!=1){
        page=(page==0)? 2:0;
        if(alarm_flag){
            alarm_cnt++;
            EEPROM_Write(4, alarm_cnt);
            alarm_flag=0;
        }
        LCD_Clear(Black);
    }
    if(key_down==2 && page==1){
        select_1++;
        select_1%=3;
    }
    if(key_down==3 || key_down==4){
        TIM3->CNT=0;
    }
    if(TIM3->CNT<10000){
        if(key_up==3){
            if(page==1){
                if(select_1==0){
                    hours++;
                    hours%=24;
                }
                if(select_1==1){
                    minutes++;
                    minutes%=60;
                }
                if(select_1==2){
                    seconds++;
                    seconds%=60;
                }
                rtc_flag=1;
            }
            if(page==2){
                if(select_2==0){
                    alarm_h++;
                    alarm_h%=24;
                }
                if(select_2==1){
                    alarm_m++;
                    alarm_m%=60;
                }
                if(select_2==2){
                    alarm_s++;
                    alarm_s%=60;
                }
                modify=1;
                alarm_flag=1;
            }
        }
        if(key_up==4){
            if(page==1){
                if(select_1==0){
                    hours--;
                    if(hours==255) hours=23;
                }
                if(select_1==1){
                    minutes--;
                    if(minutes==255) minutes=59;
                }
                if(select_1==2){
                    seconds--;
                    if(seconds==255) seconds=59;
                }
                rtc_flag=1;
            }
            if(page==2){
                if(select_2==0){
                    alarm_h--;
                    if(alarm_h==255) alarm_h=59;
                }
                if(select_2==1){
                    alarm_m--;
                    if(alarm_m==255) alarm_m=59;
                }
                if(select_2==2){
                    alarm_s--;
                    if(alarm_s==255) alarm_s=59;
                }
                modify=1;
                alarm_flag=1;
            }
        }
    }else{
        if(key_val==3){
            if(page==1){
                if(select_1==0){
                    hours++;
                    hours%=24;
                }
                if(select_1==1){
                    minutes++;
                    minutes%=60;
                }
                if(select_1==2){
                    seconds++;
                    seconds%=60;
                }
                rtc_flag=1;
            }
            if(page==2){
                if(select_2==0){
                    alarm_h++;
                    alarm_h%=24;
                }
                if(select_2==1){
                    alarm_m++;
                    alarm_m%=60;
                }
                if(select_2==2){
                    alarm_s++;
                    alarm_s%=60;
                }
                modify=1;
                alarm_flag=1;
            }
        }
        if(key_val==4){
            if(page==1){
                if(select_1==0){
                    hours--;
                    if(hours==255) hours=23;
                }
                if(select_1==1){
                    minutes--;
                    if(minutes==255) minutes=59;
                }
                if(select_1==2){
                    seconds--;
                    if(seconds==255) seconds=59;
                }
                rtc_flag=1;
            }
            if(page==2){
                if(select_2==0){
                    alarm_h--;
                    if(alarm_h==255) alarm_h=59;
                }
                if(select_2==1){
                    alarm_m--;
                    if(alarm_m==255) alarm_m=59;
                }
                if(select_2==2){
                    alarm_s--;
                    if(alarm_s==255) alarm_s=59;
                }
                modify=1;
                alarm_flag=1;
            }
        }
    }
    if(modify){
        EEPROM_Write(0, alarm_h);
        EEPROM_Write(1, alarm_m);
        EEPROM_Write(2, alarm_s);
        modify=0;
    }
}
void USART_Proc(){
    if(rtc_flag){
        sprintf(send_buff, "New RTC: %02d:%02d:%02d\r\n", hours, minutes, seconds);
        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
    }
    memset(send_buff, 0, 50);
    if(alarm_flag){
        sprintf(send_buff, "New Alarm: %02d:%02d:%02d\r\n", alarm_h, alarm_m, alarm_s);
        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        seconds++;
        if(seconds>=60){
            seconds=0;
            minutes++;
        }
        if(minutes>=60){
            minutes=0;
            hours++;
        }
        if(hours>=24){
            hours=0;
        }
    }
    if(htim->Instance==TIM4){
        led_mode^=1;
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
