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
#include "stdlib.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct info{
    double v;
    uint16_t fre;
    uint8_t duty;
}info;
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
uint8_t page=0, FP=1, TT=6, FN=0, AN=0, TN=0, para_select=0, fset_select=0, TH=30, lock=0, D2=0;
uint8_t fre_flag=0, vol_flag=0;
uint16_t F1=0, FH=2000, sec = 0, idx=0, count=0;
double vr37=0, AH=3.0, VP=0.9;
uint8_t key_val, key_up, key_down, key_last_val;
info recd[1000];
uint8_t recd_flag=0, vol_huifang=0, fre_huifang=0, led_flag=0, play_state=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LCD_Proc(void);
void LED_Proc(void);
void KEY_Proc(void);
void RECD_Proc(void);
void PWM_Proc(void);
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
  MX_TIM7_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM15_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetTextColor(White);
    LCD_SetBackColor(Black);
    // TIM
    TIM7->CNT=0;
    HAL_TIM_Base_Start(&htim7);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
    TIM4->CNT=0;
    TIM15->CNT=0;
    __HAL_TIM_CLEAR_FLAG(&htim8, TIM_FLAG_UPDATE);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    // PWM
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    TIM3->CCR2=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      KEY_Proc();
      if(recd_flag==1){
          HAL_TIM_Base_Start_IT(&htim4);
          RECD_Proc();
      }
      if(play_state==1){
          PWM_Proc();
      }else{
          TIM3->CCR2=0;
      }
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
double Get_Vol(){
    HAL_ADC_Start(&hadc2);
    uint16_t temp = HAL_ADC_GetValue(&hadc2);
    return temp*3.3/4096;
}
void RECD_Proc(){
    if(sec==0){
        HAL_TIM_Base_Stop_IT(&htim4);
        TIM4->CNT=0;
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
        recd_flag=0;
        lock=0;
    }
}
void PWM_Proc(){
    if(vol_huifang==1){
        // 电压回放
        __HAL_TIM_SET_PRESCALER(&htim3, 800-1);
        TIM3->CCR2 = (uint8_t)((90/(3.3-VP))*recd[count].v + 10 - (90/(3.3-VP))*VP);
    }else if(fre_huifang==1){
        // 脉冲信号回放
        __HAL_TIM_SET_PRESCALER(&htim3, 800000*FP/recd[count].fre-1);
        TIM3->CCR2 = recd[count].duty;
    }
}
void LCD_Proc(void){
    vr37 = Get_Vol();
    if(F1>FH && fre_flag==0){
        FN++;
        fre_flag=1;
    }else if(F1<=FH){
        fre_flag=0;
    }
    if(vr37>AH && vol_flag==0){
        AN++;
        vol_flag=1;
    }else if(vr37<=AH){
        vol_flag=0;
    }
    sprintf(text, "   %d  %d", sec, TIM4->CNT);
    LCD_DisplayStringLine(Line8, (uint8_t*)text);
    sprintf(text, "%d   %d  %d", idx, count, TIM15->CNT);
    LCD_DisplayStringLine(Line9, (uint8_t*)text);
    if(page==0){
        sprintf(text, "        DATA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     F=%d        ", F1);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     D=%d%%          ", D2);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     A=%.1f          ", vr37);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        sprintf(text, "     T=%.1f         ", 23.8);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        para_select=0;
    }else if(page==1){
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     FH=%d       ", FH);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     AH=%.1f         ", AH);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     TH=%d          ", TH);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }else if(page==2){
        sprintf(text, "        RECD        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     FN=%d         ", FN);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     AN=%d         ", AN);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     TN=%d         ", TN);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        fset_select=0;
    }else if(page==3){
        sprintf(text, "        FSET        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     FP=%d          ", FP);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     VP=%.1f         ", VP);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     TT=%d          ", TT);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
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
    if(key_down==1 && lock==0){
        page++;
        page%=4;
        LCD_Clear(Black);
    }
    if(key_down==2 && lock==0){
        if(page==0){
            lock=1;
            recd_flag=1;
            idx=0;
            sec = TT*100;
            HAL_TIM_Base_Start_IT(&htim4);
        }else if(page==1){
            para_select++;
            para_select%=3;
        }else if(page==3){
            fset_select++;
            fset_select%=3;
        }else if(page==2){
            FN=AN=TN=0;
        }
    }
    if((key_down==3 || key_down==4) && lock==0){
        TIM7->CNT=0;
    }
    if(TIM7->CNT>10000){
        if((key_val==3 || key_val==4) && lock==0){
            FH=2000;AH=3.0;TH=30;
            FP=1;VP=0.9;TT=6;
            FN=AN=TN=0;
            page=0;
        }
    }else{
        if(key_up==3 && lock==0){
            if(page==0){
                if(idx!=0){
                    TIM15->CNT=0;
                    vol_huifang=1;
                    play_state=1;
                    TIM15->CNT=0;
                    HAL_TIM_Base_Start_IT(&htim15);
                }
            }else if(page==1){
                if(para_select==0){
                    FH+=1000;
                    if(FH>10000) FH=10000;
                }else if(para_select==1){
                    AH+=0.3;
                    if(AH-3.3>0.1) AH=3.3;
                }else if(para_select==2){
                    TH+=1;
                    if(TH>80) TH=80;
                }
            }else if(page==3){
                if(fset_select==0){
                    FP+=1;
                    if(FP>10) FP=10;
                }else if(fset_select==1){
                    VP+=0.3;
                    if(VP-3.3>0.1) VP=3.3;
                }else if(fset_select==2){
                    TT+=2;
                    if(TT>10) TT=10;
                }
            }
        }
        if(key_up==4 && lock==0){
            if(page==0){
                if(idx!=0){
                    TIM15->CNT=0;
                    fre_huifang=1;
                    play_state=1;
                    TIM15->CNT=0;
                    HAL_TIM_Base_Start_IT(&htim15);
                }
            }else if(page==1){
                if(para_select==0){
                    FH-=1000;
                    if(FH<1000) FH=1000;
                }else if(para_select==1){
                    AH-=0.3;
                    if(AH<0) AH=0;
                }else if(para_select==2){
                    TH-=1;
                    if(TH==255) TH=0;
                }
            }else if(page==3){
                if(fset_select==0){
                    FP-=1;
                    if(FP<1) FP=1;
                }else if(fset_select==1){
                    VP-=0.3;
                    if(VP<0) VP=0;
                }else if(fset_select==2){
                    TT-=2;
                    if(TT<2) TT=2;
                }
            }
        }
    }
}
void LED_Show(uint8_t led, uint8_t mode){
    if(mode==0){
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}
void LED_Proc(void){
    (F1>FH)? LED_Show(0x08, 1) : LED_Show(0x08, 0);
    (vr37>AH)? LED_Show(0x10, 1) : LED_Show(0x10, 0);
    LED_Show(0x20, 1);
    if(recd_flag==1 || fre_huifang==1 || vol_huifang==1){
        HAL_TIM_Base_Start_IT(&htim8);
    }else{
        led_flag=0;
        HAL_TIM_Base_Stop_IT(&htim8);
        TIM8->CNT=0;
        __HAL_TIM_CLEAR_FLAG(&htim8, TIM_FLAG_UPDATE);
    }
    if(recd_flag==1) LED_Show(0x01, led_flag);
    else if(fre_huifang==1) LED_Show(0x02, led_flag);
    else if(vol_huifang==1) LED_Show(0x04, led_flag);
    else LED_Show(0x07, 0);
}
uint8_t cap_state=0;
uint16_t PWM_cycle, PWM_high;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        if(cap_state==0){
            PWM_cycle = TIM2->CCR2;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);  // 捕获下降沿
            cap_state=1;
        }else if(cap_state==1){
            PWM_high = TIM2->CCR2 - PWM_cycle;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);  // 捕获上升沿
            cap_state=2;
        }else if(cap_state==2){
            F1 = 1000000 / (TIM2->CCR2 - PWM_cycle);
            D2 = PWM_high*100 / (TIM2->CCR2 - PWM_cycle);
            cap_state=0;
            TIM2->CNT=0;
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM8){
        led_flag^=1;
    }
    if(htim->Instance==TIM4){
        recd[idx].duty = D2;
        recd[idx].fre = F1;
        recd[idx++].v = vr37;
        sec--;
        if(sec==0) HAL_TIM_Base_Stop_IT(htim);
    }
    if(htim->Instance==TIM15){
        count++;
        if(count==idx) {
            count=play_state=0;
            vol_huifang=fre_huifang=0;
            HAL_TIM_Base_Stop_IT(&htim15);
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
