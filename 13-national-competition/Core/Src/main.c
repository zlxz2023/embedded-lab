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
uint8_t page=0, mini_page = 0, x=1, y=1, mode=0, long_flag=0, swap_flag=0, clear_flag=0, led_flag=0;
uint16_t idx=0, cnt=0, len=0;
double V4[256], V5[256];
double A4=0, T4=0, H4=0, A5=0, H5=0, T5=0;
double V[2];
uint16_t F1;
char text[20];
uint8_t key_val, key_down, key_up, key_last_val;
uint8_t rec_data, count, rec_flag;
char send_buf[255], rec_buf[255];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void LED_Proc(void);
void KEY_Proc(void);
void PWM_Proc(void);
void USART_Proc(void);
void LED_Show(uint8_t led, uint8_t mode);
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
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM17_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    for(int i = 0;i < 256;i++){
        V4[i]=0;
        V5[i]=0;
    }
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM
    TIM7->CNT=0;
    HAL_TIM_Base_Start(&htim7);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    TIM17->CCR1=50;
    HAL_TIM_Base_Start(&htim3);
    TIM3->CNT=0;
    // ADC
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    // I2C
    I2CInit();
    if(EEPROM_Read(200)==10){
        EEPROM_Write(0, 1);
        EEPROM_Write(1, 1);
        EEPROM_Write(200, 200);
    }
    y = EEPROM_Read(0);
    x = EEPROM_Read(1);
    // UASRT
    HAL_UART_Receive_IT(&huart1, &rec_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      USART_Proc();
      KEY_Proc();
      PWM_Proc();
      LED_Proc();
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
void LED_Show(uint8_t led, uint8_t mode){
    if(mode==1){
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
    if(mode==0){
        LED_Show(0x01, 1);
        LED_Show(0x02, 0);
    }else{
        LED_Show(0x01, 0);
        LED_Show(0x02, 1);
    }
    if(swap_flag==0){
        LED_Show(0x08, 1);
    }else{
        LED_Show(0x08, 0);
    }
    if(V[0]>y * V[1]){
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_IT_UPDATE);
        HAL_TIM_Base_Start_IT(&htim4);
        LED_Show(0x04, led_flag);
    }else{
        HAL_TIM_Base_Stop_IT(&htim4);
        TIM4->CNT=0;
        led_flag=0;
    }
}
void LCD_Proc(void){
    if(swap_flag==0){
        LCD_WriteReg(R1, 0x0000);   // set SS and SM bit		  //0x0100
        LCD_WriteReg(R96, 0x2700);  // Gate Scan Line		  0xA700
    }else{
        LCD_WriteReg(R1, 0x0100);   // set SS and SM bit		  //0x0100
        LCD_WriteReg(R96, 0xA700);  // Gate Scan Line		  0xA700
    }
    if(clear_flag==1){
        LCD_Clear(Black);
        clear_flag=0;
    }
    if(page==0){
        sprintf(text, "        DATA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     PA4=%.2f       ", V[0]);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     PA5=%.2f       ", V[1]);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        sprintf(text, "     PA1=%d      ", F1);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }else if(page==1){
        sprintf(text, "        PARA        ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "     X=%d            ", x);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "     Y=%d            ", y);
        LCD_DisplayStringLine(Line4, (uint8_t*)text);
        mini_page=0;
    }else{
        if(mini_page==0){
            sprintf(text, "        REC-PA4     ");
            LCD_DisplayStringLine(Line1, (uint8_t*)text);
            sprintf(text, "     N=%d          ", cnt);
            LCD_DisplayStringLine(Line3, (uint8_t*)text);
            sprintf(text, "     A=%.2f         ", A4);
            LCD_DisplayStringLine(Line4, (uint8_t*)text);
            sprintf(text, "     T=%.2f         ", T4);
            LCD_DisplayStringLine(Line5, (uint8_t*)text);
            sprintf(text, "     H=%.2f         ", H4);
            LCD_DisplayStringLine(Line6, (uint8_t*)text);
        }else{
            sprintf(text, "        REC-PA5     ");
            LCD_DisplayStringLine(Line1, (uint8_t*)text);
            sprintf(text, "     N=%d          ", cnt);
            LCD_DisplayStringLine(Line3, (uint8_t*)text);
            sprintf(text, "     A=%.2f         ", A5);
            LCD_DisplayStringLine(Line4, (uint8_t*)text);
            sprintf(text, "     T=%.2f         ", T5);
            LCD_DisplayStringLine(Line5, (uint8_t*)text);
            sprintf(text, "     H=%.2f         ", H5);
            LCD_DisplayStringLine(Line6, (uint8_t*)text);
        }
    }
}
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
        page++;
        page %= 3;
        LCD_Clear(Black);
    }
    if(key_down==2){
        x++;
        if(x>4) x=1;
        EEPROM_Write(1, x);
    }
    if(key_down==3){
        y++;
        if(y>4) y=1;
        EEPROM_Write(0, y);
    }
    if(key_down==4 && page==0){
        for(int i = 0;i < 2;i++){
            HAL_ADC_Start(&hadc2);
            if(HAL_ADC_PollForConversion(&hadc2, 10)==HAL_OK){
                V[i] = 3.3 * HAL_ADC_GetValue(&hadc2) / 4096;
            }
        }
        cnt++;
        if(len==0){
            A4=T4=H4=V[0];
            A5=T5=H5=V[1];
        }else{
            if(V[0] > A4) A4=V[0];
            if(V[0] < T4) T4=V[0];
            if(V[1] > A5) A5=V[1];
            if(V[1] < T5) T5=V[1];
            if(len==256){
                double sum4 = H4*len-V4[idx]+V[0], sum5 = H5*len-V5[idx]+V[1];
                H4 = sum4 / len;
                H5 = sum5 / len;
            }else{
                double sum4 = H4*len+V[0], sum5 = H5*len+V[1];
                H4 = sum4 / (len+1);
                H5 = sum5 / (len+1);
            }
        }
        V4[idx]=V[0];
        V5[idx]=V[1];
        idx++;
        idx %= 256;
        len++;
        if(len>256) len=256;
    }
    if(key_down==4 && page==1){
        // mode==0---±¶Æµ
        mode^=1;
    }
    if(key_down==4 && page==2){
        TIM7->CNT=0;
    }
    if(TIM7->CNT<10000){
        if(key_up==4 && page==2){
            mini_page^=1;
        }
    }else{
        if(key_val==4 && page==2){
            long_flag = 1;
        }
    }
    if(key_up==4 && long_flag==1){
        long_flag=0;
        cnt=0;
        A4 = 0; T4 = 0; H4 = 0;
        A5 = 0; T5 = 0; H5 = 0;
        idx = 0;
        len = 0;
        memset(V4, 0, sizeof(V4));
        memset(V5, 0, sizeof(V5));
    }
}
void PWM_Proc(void){
    uint32_t f;
    if(mode==0) {
        f = F1 * x;
    }else{
        f = F1 / x;
    }
    __HAL_TIM_SET_PRESCALER(&htim17, 800000/f-1);
}
void USART_Proc(void){
    if(rec_flag==1){
        if(TIM3->CNT>15){
            if(strcmp(rec_buf, "X")==0){
                sprintf(send_buf, "X:%d", x);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, sizeof(send_buf), 50);
            }else if(strcmp(rec_buf, "Y")==0){
                sprintf(send_buf, "Y:%d", y);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, sizeof(send_buf), 50);
            }else if(strcmp(rec_buf, "PA1")==0){
                sprintf(send_buf, "PA1:%d", F1);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, sizeof(send_buf), 50);
            }else if(strcmp(rec_buf, "PA4")==0){
                sprintf(send_buf, "PA4:%.2f", V[0]);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, sizeof(send_buf), 50);
            }else if(strcmp(rec_buf, "PA5")==0){
                sprintf(send_buf, "PA5:%.2f", V[1]);
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buf, sizeof(send_buf), 50);
            }else if(strcmp(rec_buf, "#")==0){
                swap_flag^=1;
                clear_flag=1;
            }
            rec_flag=0;
            count=0;
            memset(rec_buf, 0, sizeof(rec_buf));
            HAL_UART_Receive_IT(&huart1, &rec_data, 1);
        }
    }
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        uint32_t cap = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
        TIM2->CNT=0;
        F1 = 80000000/(80*cap);
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        rec_buf[count]=rec_data;
        count++;
        rec_flag=1;
        TIM3->CNT=0;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM4){
        led_flag^=1;
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
