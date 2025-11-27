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
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "lcd.h"
#include "stdlib.h"
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
uint8_t page, mini_page, nda, ndb, nha, nhb, line, lowa_flag, lowb_flag;
uint16_t ta, tb, pd=1000, ph=5000;
int px, fa, fb;
uint8_t key_val,key_up,key_down,key_last_val;
uint8_t long_flag=0, rear=0;  // ¶ÓÁÐÎ²²¿
int num_a[30] = {0}, num_b[30] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
void LED_Proc(void);
void DH(void);
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
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    TIM4->CNT=0;
    HAL_TIM_Base_Start(&htim4);
    if(fa<ph) lowa_flag=1;
    if(fb<ph) lowb_flag=1;
    TIM8->CNT=0;
    HAL_TIM_Base_Start_IT(&htim8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      KEY_Proc();
      if(lowa_flag && fa > ph){
          lowa_flag=0;
          nha++;
      }
      if(fa < ph) lowa_flag=1;
      if(lowb_flag && fb > ph){
          lowb_flag=0;
          nhb++;
      }
      if(fb < ph) lowb_flag=1;
      LED_Proc();
      DH();
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
        if(mini_page==0){
            sprintf(text, "        DATA        ");
            LCD_DisplayStringLine(Line1, (uint8_t*)text);
            if(fa>0){
                if(fa<=1000) sprintf(text, "     A=%dHz       ", fa);
                else if(fa>1000) sprintf(text, "     A=%.2fKHz", fa*1.0/1000);
            }else{
                sprintf(text, "     A=NULL         ");
            }
            LCD_DisplayStringLine(Line3, (uint8_t*)text);
            if(fb>0){
                if(fb<=1000) sprintf(text, "     B=%dHz       ", fb);
                else if(fb>1000) sprintf(text, "     B=%.2fKHz", fb*1.0/1000);
            }else{
                sprintf(text, "     B=NULL         ");
            }
            LCD_DisplayStringLine(Line4, (uint8_t*)text);
        }else{
            sprintf(text, "        DATA        ");
            LCD_DisplayStringLine(Line1, (uint8_t*)text);
            if(fa>0){
                ta = (1.0/fa)*1000000;
                if(ta<=1000) sprintf(text, "     A=%duS       ", ta);
                else sprintf(text, "     A=%.2fmS", ta*1.0/1000);
            }else{
                sprintf(text, "     A=NULL         ");
            }
            LCD_DisplayStringLine(Line3, (uint8_t*)text);
            if(fb>0){
                tb = (1.0/fb)*1000000;
                if(tb<=1000) sprintf(text, "     B=%duS       ", tb);
                else sprintf(text, "     B=%.2fmS", tb*1.0/1000);
            }else{
                sprintf(text, "     B=NULL         ");
            }
            LCD_DisplayStringLine(Line4, (uint8_t*)text);
        }
        line=0;
    }else if(page==1){
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     PD=%dHz      ", pd);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     PH=%dHz     ", ph);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     PX=%dHz     ", px);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }else{
        sprintf(text, "        RECD        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     NDA=%d      ", nda);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     NDB=%d        ", ndb);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     NHA=%d        ", nha);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        sprintf(text, "     NHB=%d        ", nhb);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        mini_page=0;
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
    if(key_down==1 && page==1){
        if(line==0){
            pd+=100;
            if(pd>1000) pd=1000;
        }
        if(line==1){
            ph+=100;
            if(ph>10000) ph=10000;
        }
        if(line==2){
            px+=100;
            if(px>1000) px=1000;
        }
    }
    if(key_down==2 && page==1){
        if(line==0){
            pd-=100;
            if(pd<100) pd=100;
        }
        if(line==1){
            ph-=100;
            if(ph<1000) ph=1000;
        }
        if(line==2){
            px-=100;
            if(px<-1000) px=-1000;
        }
    }
    if(page==1){
        if(key_down==3){
            line++;
            line%=3;
        }
    }
    if(page==0){
        if(key_down==3){
            mini_page^=1;
        }
    }
    if(page==2){
        if(key_down==3){
            TIM4->CNT=0;
        }
        if(key_val==3 && long_flag==0){
            if(TIM4->CNT>=10000-1){
                long_flag=1;
            }
        }
        if(long_flag && key_up==3){
            long_flag=0;
            nha=nhb=nda=ndb=0;
        }
    }
    if(key_down==4){
        page++;
        page%=3;
        LCD_Clear(Black);
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
    if(page==0) LED_Show(0x01, 1);
    else LED_Show(0x01, 0);
    if(fa>ph) LED_Show(0x02, 1);
    else LED_Show(0x02, 0);
    if(fb>ph) LED_Show(0x04, 1);
    else LED_Show(0x04, 0);
    if(nda>=3 || ndb>=3) LED_Show(0x80, 1);
    else LED_Show(0x80, 0);
}
void DH(void){
    if(rear==30){
        int max_a=num_a[0];
        int min_a=num_a[0];
        int max_b=num_b[0];
        int min_b=num_b[0];
        for(int i=1;i<30;i++){
            if(max_a<num_a[i]) max_a=num_a[i];
            if(min_a>num_a[i]) min_a=num_a[i];
            if(max_b<num_b[i]) max_b=num_b[i];
            if(min_b>num_b[i]) min_b=num_b[i];
        }
        if(max_a-min_a>pd) nda++;
        if(max_b-min_b>pd) ndb++;
        memset(num_a, 0, sizeof(num_a));
        memset(num_b, 0, sizeof(num_b));
        rear=0;
    }
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        uint32_t cap = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        TIM2->CNT=0;
        fa = 1000000/cap + px;
    }
    if(htim->Instance==TIM3){
        uint32_t cap = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        TIM3->CNT=0;
        fb = 1000000/cap + px;
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM8){
        num_a[rear]=fa;
        num_b[rear]=fb;
        rear++;
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
