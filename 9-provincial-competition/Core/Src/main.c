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
char* state[4] = {"Standby", "Running", "Pause", "Setting"};
uint8_t page=1; // 定义显示的LCD页码
uint8_t num[3]; // 存储时间
uint8_t tim_state, tim_temp;  // 标识定时器工作状态 0-关闭 1-开启 2-暂停 3-设置
uint8_t key_val, key_up, key_down, key_last_val;
uint8_t led_mode;
uint8_t first_1=1, first_2=1, first_3=1, first_4=1, first_5=1;  // 记录是否第一次
uint8_t B2_flag;  // 记录指向哪一个 0、1、2
uint8_t hours, minutes, seconds;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void TIM_Proc(void);
void LED_Show(uint8_t led, uint8_t led_mode);
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
  MX_TIM16_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  /*    TIM2-----用于LED
        TIM3-----用于按键
        TIM4-----用于计时
        TIM16----用于PWM PA6
  */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // I2C
    I2CInit();
    // TIM
    HAL_TIM_Base_Start(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      KEY_Proc();
      TIM_Proc();
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
void LED_Show(uint8_t led, uint8_t led_mode){
    HAL_GPIO_WritePin(GPIOC, 0xfe<<8, GPIO_PIN_SET);
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
void LCD_Proc(void){
    if(page==1){
        if(first_1){
            EEPROM_Read(0, num);
            first_1=0;
        }
        sprintf(text, "    No %d            ", page);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "      %s      ", state[tim_state]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        if(tim_state==3 && B2_flag==0){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 6)) , num[0]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , num[0]%10 + 0x30);
        if(tim_state==3 && B2_flag==1){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 9)) , num[1]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , num[1]%10 + 0x30);
        if(tim_state==3 && B2_flag==2){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 12)) , num[2]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , num[2]%10 + 0x30);
        LCD_SetBackColor(Black);
        LCD_DisplayChar(Line4 , (320 - (16 * 8)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 11)) , ':');
    }
    if(page==2){
        if(first_2){
            EEPROM_Read(3, num);
            first_2=0;
        }
        sprintf(text, "    No %d            ", page);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "      %s      ", state[tim_state]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        if(tim_state==3 && B2_flag==0){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 6)) , num[0]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , num[0]%10 + 0x30);
        if(tim_state==3 && B2_flag==1){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 9)) , num[1]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , num[1]%10 + 0x30);
        if(tim_state==3 && B2_flag==2){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 12)) , num[2]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , num[2]%10 + 0x30);
        LCD_SetBackColor(Black);
        LCD_DisplayChar(Line4 , (320 - (16 * 8)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 11)) , ':');
    }
    if(page==3){
        if(first_3){
            EEPROM_Read(6, num);
            first_3=0;
        }
        sprintf(text, "    No %d            ", page);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "      %s      ", state[tim_state]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        if(tim_state==3 && B2_flag==0){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 6)) , num[0]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , num[0]%10 + 0x30);
        if(tim_state==3 && B2_flag==1){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 9)) , num[1]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , num[1]%10 + 0x30);
        if(tim_state==3 && B2_flag==2){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 12)) , num[2]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , num[2]%10 + 0x30);
        LCD_SetBackColor(Black);
        LCD_DisplayChar(Line4 , (320 - (16 * 8)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 11)) , ':');
    }
    if(page==4){
        if(first_4){
            EEPROM_Read(9, num);
            first_4=0;
        }
        sprintf(text, "    No %d            ", page);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "      %s      ", state[tim_state]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        if(tim_state==3 && B2_flag==0){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 6)) , num[0]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , num[0]%10 + 0x30);
        if(tim_state==3 && B2_flag==1){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 9)) , num[1]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , num[1]%10 + 0x30);
        if(tim_state==3 && B2_flag==2){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 12)) , num[2]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , num[2]%10 + 0x30);
        LCD_SetBackColor(Black);
        LCD_DisplayChar(Line4 , (320 - (16 * 8)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 11)) , ':');
    }
    if(page==5){
        if(first_5){
            EEPROM_Read(12, num);
            first_5=0;
        }
        sprintf(text, "    No %d            ", page);
        LCD_DisplayStringLine(Line2, (uint8_t*)text);
        sprintf(text, "      %s      ", state[tim_state]);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        if(tim_state==3 && B2_flag==0){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 6)) , num[0]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 7)) , num[0]%10 + 0x30);
        if(tim_state==3 && B2_flag==1){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 9)) , num[1]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 10)) , num[1]%10 + 0x30);
        if(tim_state==3 && B2_flag==2){
            LCD_SetBackColor(Red);
        }else{
            LCD_SetBackColor(Black);
        }
        LCD_DisplayChar(Line4 , (320 - (16 * 12)) , num[2]/10 + 0x30);
        LCD_DisplayChar(Line4 , (320 - (16 * 13)) , num[2]%10 + 0x30);
        LCD_SetBackColor(Black);
        LCD_DisplayChar(Line4 , (320 - (16 * 8)) , ':');
        LCD_DisplayChar(Line4 , (320 - (16 * 11)) , ':');
    }
}
uint8_t KEY_Scan(){
    uint8_t key_value;
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
void KEY_Proc(){
    key_val=KEY_Scan();
    key_down=key_val&(key_last_val^key_val);
    key_up=~key_val&(key_last_val^key_val);
    tim_temp=tim_state;
    if(key_val==1 && key_last_val==0){
        page++;
        if(page>5) page=1;
        if(page==1){
            first_1=1;
        }
        if(page==2){
            first_2=1;
        }
        if(page==3){
            first_3=1;
        }
        if(page==4){
            first_4=1;
        }
        if(page==5){
            first_5=1;
        }
        LCD_Clear(Black);
    }
    if(key_down && key_down!=1){
        TIM3->CNT=0;
    }
    if(TIM3->CNT<8000){  // 短按 0.8s
        if(key_up == 2){
            tim_state=3;
            B2_flag++;
            if(B2_flag>2) B2_flag=0; 
        }
        if(key_up == 3){
            if(B2_flag==0){  // 时
                num[0]++;
                if(num[0]>23) num[0]=0;
            }
            if(B2_flag==1){  // 分
                num[1]++;
                if(num[1]>59) num[1]=0;
            }
            if(B2_flag==2){  // 秒
                num[2]++;
                if(num[2]>59) num[2]=0;
            }
        }
        if(key_up == 4){
            if(tim_state==0){
                tim_state=1;
            }
            if(tim_state==1 && tim_temp==1){
                tim_state=2;
            }
            if(tim_state==2 && tim_temp==2){
                tim_state=1;
            }
        }
    }else{  // 长按 0.8s
        if(key_val == 2){
            hours=num[0];minutes=num[1];seconds=num[2];
            if(page==1){
                EEPROM_Write(0, hours);
                EEPROM_Write(1, minutes);
                EEPROM_Write(2, seconds);
                first_1=1;
            }
            if(page==2){
                EEPROM_Write(3, hours);
                EEPROM_Write(4, minutes);
                EEPROM_Write(5, seconds);
                first_2=1;
            }
            if(page==3){
                EEPROM_Write(6, hours);
                EEPROM_Write(7, minutes);
                EEPROM_Write(8, seconds);
                first_3=1;
            }
            if(page==4){
                EEPROM_Write(9, hours);
                EEPROM_Write(10, minutes);
                EEPROM_Write(11, seconds);
                first_4=1;
            }
            if(page==5){
                EEPROM_Write(12, hours);
                EEPROM_Write(13, minutes);
                EEPROM_Write(14, seconds);
                first_5=1;
            }
            tim_state=0;
        }
        if(key_val == 3){
            while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)==GPIO_PIN_RESET){
                LCD_Proc();
                if(B2_flag==0){  // 时
                    num[0]++;
                    if(num[0]>23) num[0]=0;
                }
                if(B2_flag==1){  // 分
                    num[1]++;
                    if(num[1]>59) num[1]=0;
                }
                if(B2_flag==2){  // 秒
                    num[2]++;
                    if(num[2]>59) num[2]=0;
                }
            }
        }
        if(key_val == 4){
            tim_state=0;
        }
    }
    key_last_val=key_val;
}
void TIM_Proc(){
    if(tim_state==1){
        HAL_TIM_Base_Start_IT(&htim4);
        HAL_TIM_Base_Start_IT(&htim2);
        LED_Show(0x01, led_mode);
        HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
        TIM16->CCR1=80;
    }else{
        HAL_TIM_Base_Stop_IT(&htim2);
        LED_Show(0x01, 0);
        led_mode=0;
        TIM2->CNT=0;
        HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim4);
    }
    if(num[0] == 0 && num[1] == 0 && num[2] == 0){
        tim_state=0;
        HAL_TIM_Base_Stop_IT(&htim2);
        LED_Show(0x01, 0);
        led_mode=0;
        TIM2->CNT=0;
        HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim4);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        led_mode ^= 1;
    }
    if(htim->Instance==TIM4){
        num[2]--;
        if(num[2]==255){
            num[2]=59;
            num[1]--;
        }
        if(num[1]==255){
            num[1]=59;
            num[0]--;
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
