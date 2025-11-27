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
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"
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
uint8_t rec_data, count=0, rec_flag=0;
uint16_t dst_index=0, rn_index=0;
char rec_buf[1000], send_buf[1000];
uint16_t dst[400];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
uint8_t KEY_Scan(void);
void KEY_Proc(void);
void LED_Proc(void);
void USART_Proc(void);
void PWM_Proc(void);
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
  MX_USART1_UART_Init();
  MX_TIM17_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Base_Start(&htim17);
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    TIM16->CCR1=0;
    TIM3->CCR2=0;
    // USART1
    HAL_UART_Receive_IT(&huart1, &rec_data, 1);
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      KEY_Proc();
      USART_Proc();
      PWM_Proc();
      LCD_Proc();
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
char text[20];
char* state[3] = {"Idle", "Busy", "Wait"};
uint8_t rn = 0, page=0, b = 10, para_select=0, sidx=0;
uint16_t tt = 0, cp[2]={0}, tp[2]={0};
double se = 0, r = 1.0, ts = 0;
void LCD_Proc(void){
    sprintf(text, "        %d       ", TIM17->CNT);
        LCD_DisplayStringLine(Line9, (uint8_t*)text);
    if(page==0){
        sprintf(text, "        DATA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     ST=%s        ", state[sidx]);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     CP=%d,%d     ", cp[0], cp[1]);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     TP=%d,%d     ", tp[0], tp[1]);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        sprintf(text, "     SE=%.1f        ", se);
        LCD_DisplayStringLine(Line6, (uint8_t*)text);
        sprintf(text, "     RN=%d        ", rn);
        LCD_DisplayStringLine(Line7, (uint8_t*)text);
        para_select=0;
    }else if(page==1){
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     R=%.1f          ", r);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     B=%d          ", b);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }else if(page==2){
        sprintf(text, "        RECD        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     TS=%.1f      ", ts);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     TT=%d      ", tt);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
    }
}
uint8_t key_val, key_down, key_up, key_last_val;
uint8_t KEY_Scan(void){
    uint8_t key_value = 0;
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
        if(dst_index!=0){
            sidx++;
            if(sidx==3) sidx=1;
        }
    }
    if(key_down==2){
        page++;
        page%=3;
        LCD_Clear(Black);
    }
    if(key_down==3 && page==1){
        para_select^=1;
    }
    if(key_down==4 && page==1){
        if(para_select==0){
            r+=0.1;
            if(r-2.0>0.001) r=1.0;
        }else{
            b+=10;
            if(b>100) b=10;
        }
    }
}
uint8_t run_flag=0;
double last_dis=0, dis=0;
uint32_t time=0;
uint16_t last_x, last_y;
void Work(void){
    if(sidx==1 && dst_index!=0){
        if(run_flag==0){
            tp[0]=dst[rn_index++];
            tp[1]=dst[rn_index++];
            run_flag=1;
            last_dis=sqrt(pow(tp[0]-cp[0], 2)+pow(tp[1]-cp[1], 2));
            last_x = cp[0];
            last_y = cp[1];
        }
        double se_x = se*abs(tp[0]-cp[0])/last_dis;
        double se_y = se*abs(tp[1]-cp[1])/last_dis;
        cp[0] += se_x*time/1000;
        cp[1] += se_y*time/1000;
        dis = sqrt(pow(cp[0]-last_x, 2)+pow(cp[1]-last_y, 2));
        if(dis > last_dis){
            cp[0]=tp[0];
            cp[1]=tp[1];
            run_flag=0;
            ts += last_dis;
            tt += time/1000;
            time=0;
        }
    }
}
void LED_Proc(void);
int delete_x, delete_y, mode=0;
void USART_Proc(void){
    if(TIM17->CNT>15){
        if(rec_flag==1){
            // ´¦ÀíÂß¼­
            if(strcmp(rec_buf, "?")==0){
                sprintf(send_buf, "%s", state[sidx]);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            }else if(strcmp(rec_buf, "#")==0){
                sprintf(send_buf, "(%d,%d)", cp[0], cp[1]);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            }else if(rec_buf[0]=='{'){
                sscanf(rec_buf, "{%d,%d}", &delete_x, &delete_y);
                sprintf(send_buf, "{%d,%d}", delete_x, delete_y);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            }else if(rec_buf[0]=='('){  // (1,1,1,1,...)
                if(sidx!=0){
                    sprintf(send_buf, "Busy");
                    HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
                }else{
                    char temp[1000];
                    for(int i = 1;i<strlen(rec_buf)-1; i++){
                        temp[i-1] = rec_buf[i];
                    }
                    char* token;
                    token = strtok(temp, ",");
                    while(token!=NULL){
                        dst[dst_index++]=atoi(token);
                        token = strtok(NULL, ",");
                    }
                    sprintf(send_buf, "Got it");
                    HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
                }
            }else if(rec_buf[0]=='['){
                if(rec_buf[2]!=']'){
                    sprintf(send_buf, "Error");
                    HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
                }else{
                    sscanf(rec_buf, "[%d]", &mode);
                    if(mode>4){
                        sprintf(send_buf, "Error");
                        HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
                    }else{
                        sidx==1? sprintf(send_buf, "Got it") : sprintf(send_buf, "Device offline");
                        HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
                    }
                }
            }else{
                sprintf(send_buf, "Error");
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, strlen(send_buf), 50);
            }
            memset(rec_buf, 0, sizeof(rec_buf));
            count=0;
            rec_flag=0;
            HAL_UART_Receive_IT(&huart1, &rec_data, 1);
        }
    }
}
void PWM_Proc(void){
    if(sidx==1){
        if(mode==1){
            __HAL_TIM_SET_PRESCALER(&htim16, 800-1);
            __HAL_TIM_SET_PRESCALER(&htim3, 800-1);
        }else if(mode==2){
            __HAL_TIM_SET_PRESCALER(&htim16, 200-1);
            __HAL_TIM_SET_PRESCALER(&htim3, 800-1);
        }else if(mode==3){
            __HAL_TIM_SET_PRESCALER(&htim16, 800-1);
            __HAL_TIM_SET_PRESCALER(&htim3, 200-1);
        }else if(mode==4){
            __HAL_TIM_SET_PRESCALER(&htim16, 200-1);
            __HAL_TIM_SET_PRESCALER(&htim3, 200-1);
        }
        double vol = HAL_ADC_GetValue(&hadc2)*3.3/4096;
        if(vol<0.5){
            TIM16->CCR1=10;
            TIM3->CCR2=5;
        }else if(vol>=0.5 && vol < 1.0){
            TIM16->CCR1=30;
            TIM3->CCR2=25;
        }else if(vol>=1.0 && vol<1.5){
            TIM16->CCR1=50;
            TIM3->CCR2=45;
        }else if(vol>=1.5 && vol<2.0){
            TIM16->CCR1=70;
            TIM3->CCR2=65;
        }else if(vol>=2.0 && vol<2.5){
            TIM16->CCR1=90;
            TIM3->CCR2=85;
        }else if(vol>=2.5){
            TIM16->CCR1=95;
            TIM3->CCR2=90;
        }
    }else{
        TIM16->CCR1=0;
        TIM3->CCR2=0;
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        rec_buf[count]=rec_data;
        count++;
        rec_flag=1;
        TIM17->CNT=0;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
    }
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    uint32_t temp = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    uint32_t fre = 80000000/(80*temp);
    if(sidx==1){
        se = 3.14*r*fre/100 + b;
    }else{
        se=0;
    }
}
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
    time++;
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
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
