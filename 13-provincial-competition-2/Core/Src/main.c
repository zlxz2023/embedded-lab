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
#include "i2c_hal.h"
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
uint8_t cnt_x=10, cnt_y=10, buy_x=0, buy_y=0;double price_x=1.0, price_y=1.0;
uint8_t page=0;
uint8_t key_val, key_down, key_up, key_last_val;
char rec_buf[255], send_buf[255];uint8_t rec_data, count, rec_flag;
uint8_t led1_flag, led2_flag, sec_5=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void LED_Proc(void);
void KEY_Proc(void);
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
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // I2C
    I2CInit();
    cnt_x=EEPROM_Read(0);
    cnt_y=EEPROM_Read(1);
    price_x=EEPROM_Read(2)*1.0/10;
    price_y=EEPROM_Read(3)*1.0/10;
    // USART
    HAL_UART_Receive_IT(&huart1, &rec_data, 1);
    // TIM
    HAL_TIM_Base_Start(&htim8);
    TIM4->CNT=0;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    TIM2->CCR2=5;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      LED_Proc();
      KEY_Proc();
      USART_Proc();
      if(cnt_x==0 && cnt_y==0){
          HAL_TIM_Base_Start_IT(&htim4);
      }else{
          HAL_TIM_Base_Stop_IT(&htim4);
          led2_flag=0;
          TIM4->CNT=0;
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
void LCD_Proc(void){
    if(page==0){
        sprintf(text, "        SHOP        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     X:%d          ", buy_x);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     Y:%d          ", buy_y);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }else if(page==1){
        sprintf(text, "        PRICE       ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     X:%.1f          ", price_x);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     Y:%.1f          ", price_y);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }else{
        sprintf(text, "        REP         ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     X:%d          ", cnt_x);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     Y:%d          ", cnt_y);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }
    sprintf(text, "     Y:%d          ", TIM2->CCR2);
    LCD_DisplayStringLine(Line8, (uint8_t*)text);
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
    key_last_val=key_val;
    if(key_down==1){
        page++;
        page%=3;
        LCD_Clear(Black);
    }
    if(key_down==2){
        if(page==0){
            buy_x++;
            if(buy_x > cnt_x) buy_x=0;
        }else if(page==1){
            price_x += 0.1;
            if(price_x >= 2.1) price_x=1.0;
            EEPROM_Write(2, (uint8_t)(price_x*10));
        }else{
            cnt_x++;
            EEPROM_Write(0, cnt_x);
        }
    }
    if(key_down==3){
        if(page==0){
            buy_y++;
            if(buy_y > cnt_y) buy_y=0;
        }else if(page==1){
            price_y += 0.1;
            if(price_y >= 2.1) price_y=1.0;
            EEPROM_Write(3, (uint8_t)(price_y*10));
        }else{
            cnt_y++;
            EEPROM_Write(1, cnt_y);
        }
    }
    if(key_down==4){
        if(page==0){
            if(buy_x!=0){
                cnt_x=cnt_x-buy_x;
                EEPROM_Write(0, cnt_x);
            }
            if(buy_y!=0){
                cnt_y=cnt_y-buy_y;
                EEPROM_Write(1, cnt_y);
            }
            sprintf(send_buf, "X:%d,Y:%d,Z:%.1f\r\n", buy_x, buy_y, buy_x*price_x+buy_y*price_y);
            HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            led1_flag=1;
            TIM2->CCR2=30;
            TIM3->CNT=0;
            HAL_TIM_Base_Start_IT(&htim3);
            buy_x=0;buy_y=0;
        }
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
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        rec_buf[count]=rec_data;
        count++;
        TIM8->CNT=0;
        rec_flag=1;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
    }
}
void USART_Proc(void){
    if(rec_flag){
        if(TIM8->CNT>15){
            if(strcmp(rec_buf, "?")==0){
                sprintf(send_buf, "X:%.1f,Y:%.1f\r\n", price_x, price_y);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            }
            memset(rec_buf, 0, sizeof(rec_buf));
            count=0;
            rec_flag=0;
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM3){
        sec_5++;
        if(sec_5==5){
            led1_flag=0;
            TIM2->CCR2=5;
            sec_5=0;
            HAL_TIM_Base_Stop_IT(htim);
            TIM3->CNT=0;
        }
    }
    if(htim->Instance==TIM4){
        led2_flag^=1;
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
