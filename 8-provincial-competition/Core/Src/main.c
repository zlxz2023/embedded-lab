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
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
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
RTC_TimeTypeDef my_time;
RTC_DateTypeDef my_date;
uint8_t B1_State, B1_Last_State;
uint8_t B2_State, B2_Last_State;
uint8_t B3_State, B3_Last_State;
uint8_t B4_State, B4_Last_State;
char text[20];
uint8_t first=1; // 判断是否是第一次
uint8_t led[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
uint8_t go_up[3], go_down[3];  // 存储上行和下行楼层
uint8_t up_cnt, down_cnt, up_cnt1, down_cnt1;
uint8_t platform_num=1;  // 目前实际楼层数目
uint8_t platform_state=0;  // 目前电机工作状态
uint8_t flag1, flag2, flag3, flag4;
uint8_t up_flag, down_flag;  // 上行下行标志位
uint8_t time;  // 表示TIM2的计数值
uint8_t sec_1, sec_2, sec_6;  // 1s、2s、6s的标志位
uint8_t led_up=4, led_down=7;
uint8_t arrive_flag=0;  // 标识是否抵达楼层
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LED_Show(uint8_t led, uint8_t led_mode);
void lcd_interface(void);
void get_rtcvalue(void);
void KEY_Scan(void);
void flag_judge(void);
void lift_run(void);
void door_open(void);
void door_close(void);
void LCD_Shanshuo(void);
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
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    sprintf (text, "          %d         ",platform_num);
    LCD_DisplayStringLine(Line4, (uint8_t*)text);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
      lcd_interface();
      KEY_Scan();
      flag_judge();
      lift_run();
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
    if(led_mode==1){
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}
/*获取rtc函数*/
void get_rtcvalue(){
	HAL_RTC_GetTime(&hrtc, &my_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &my_date, RTC_FORMAT_BIN);
}
void lcd_interface(){
	get_rtcvalue();
	sprintf (text, "    Now Plantform   ");
    LCD_DisplayStringLine(Line2, (uint8_t*)text);
//	sprintf (text, "          %d         ",platform_num);
//    LCD_DisplayStringLine(Line4, (uint8_t*)text);
	sprintf (text, "    %02d : %02d : %02d    ",my_time.Hours,	my_time.Minutes, my_time.Seconds);
	LCD_DisplayStringLine(Line7, (uint8_t*)text);
}
void KEY_Scan(){
    // 只有电机未工作时才能检测
    if(platform_state==0){
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);  // 清除定时器中断标志位
        HAL_TIM_Base_Start_IT(&htim2);
        TIM2->CNT=0;
        sec_1=0;
        flag1=0;flag2=0;flag3=0;flag4=0;
        while(1){
            lcd_interface();
            B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
            B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
            B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
            B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
            if(B1_State==0 && B1_Last_State==1 && platform_num!=1 && flag1==0){
                go_down[down_cnt]=1;
                down_cnt++;
                flag1=1;
                TIM2->CNT=0;
                LED_Show(led[0], 1);
            }
            if(B2_State==0 && B2_Last_State==1&& platform_num!=2 && flag2==0){
                if(platform_num>2){
                    go_down[down_cnt]=2;
                    down_cnt++;
                }else{
                    go_up[up_cnt]=2;
                    up_cnt++;
                }
                flag2=1;
                TIM2->CNT=0;
                LED_Show(led[1], 1);
            }
            if(B3_State==0 && B3_Last_State==1 && platform_num!=3 && flag3==0){
                if(platform_num<3){
                    go_up[up_cnt]=3;
                    up_cnt++;
                }else{
                    go_down[down_cnt]=3;
                    down_cnt++;
                }
                flag3=1;
                TIM2->CNT=0;
                LED_Show(led[2], 1);
            }
            if(B4_State==0 && B4_Last_State==1 && platform_num!=4 && flag4==0){
                go_up[up_cnt]=4;
                up_cnt++;
                flag4=1;
                TIM2->CNT=0;
                LED_Show(led[3], 1);
            }
            B1_Last_State=B1_State;
            B2_Last_State=B2_State;
            B3_Last_State=B3_State;
            B4_Last_State=B4_State;
            if(sec_1==1 && ( strlen((char*)go_up)>0 || strlen((char*)go_down)>0 )){
                HAL_TIM_Base_Stop_IT(&htim2);
                TIM2->CNT=0;
                sec_1=0;
                platform_state=1;
                for (uint8_t i = 0; i < up_cnt; ++i){
                    for (uint8_t j = i+1; j < up_cnt; ++j){
                        if(go_up[i] > go_up[j]){
                            uint8_t temp = go_up[i];
                            go_up[i] = go_up[j];
                            go_up[j] = temp;
                        }
                    }
                }
                for (uint8_t i = 0; i < down_cnt; ++i){
                    for (uint8_t j = i+1; j < down_cnt; ++j){
                        if(go_down[i] < go_down[j]){
                            uint8_t temp = go_down[i];
                            go_down[i] = go_down[j];
                            go_down[j] = temp;
                        }
                    }
                }
                break;
            }
        }
    }
}
void flag_judge(void){
    if(strlen((char*)go_up)>0){
        up_flag=1;
        down_flag=0;
    }else if(strlen((char*)go_up)==0 && strlen((char*)go_down)>0){
        up_flag=0;
        down_flag=1;
    }else{
        up_flag=0;
        down_flag=0;
    }
}
void door_open(){
    sec_2=0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    TIM17->CCR1=60;
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim3);
    TIM3->CNT=0;
    while(sec_2==0)
        lcd_interface();
    HAL_TIM_Base_Stop_IT(&htim3);
    TIM3->CNT=0;
}
void door_close(){
    sec_2=0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    TIM17->CCR1=50;
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim3);
    TIM3->CNT=0;
    while(sec_2==0)
        lcd_interface();
    HAL_TIM_Base_Stop_IT(&htim3);
    TIM3->CNT=0;
}
void wait(){
    sec_2=0;
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim3);
    TIM3->CNT=0;
    while(sec_2==0)
        lcd_interface();
    HAL_TIM_Base_Stop_IT(&htim3);
    TIM3->CNT=0;
}
void LCD_Shanshuo(){
    for(int i = 0; i < 2; i++){
        sprintf (text, "          %d         ",platform_num);
        LCD_DisplayStringLine(Line4, (uint8_t*)text); // 显示楼层号
        lcd_interface();
        HAL_Delay(250); // 延时500ms
        lcd_interface();
        LCD_ClearLine(Line4); // 清除楼层号显示
        HAL_Delay(250); // 延时500ms
        lcd_interface();
    }
    // 再次显示楼层号
    sprintf (text, "          %d         ",platform_num);
    LCD_DisplayStringLine(Line4, (uint8_t*)text);
}
void lift_run(void){
    // 只有电机正在运转才能执行
    if(platform_state==1){
        if(first){
            first=0;
            door_close();
        }
        time=0;
        sec_6=0;
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
        HAL_TIM_Base_Start_IT(&htim2);
        TIM2->CNT=0;
        while(1){
            lcd_interface();
            // 上行
            if(up_flag==1 && down_flag==0){
                LED_Show(led[led_up++], 1);
                HAL_Delay(10);
                LED_Show(0xf0, 0);
                led_up = led_up>7?4:led_up;
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
                HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
                TIM16->CCR1=80;
            }
            // 下行
            if(up_flag==0 && down_flag==1){
                LED_Show(led[led_down--], 1);
                HAL_Delay(10);
                LED_Show(0xf0, 0);
                led_down = led_down<4?7:led_down;
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
                HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
                TIM16->CCR1=60;
            }
            if(sec_6==1 && ( strlen((char*)go_up)>0 || strlen((char*)go_down)>0 )){
                HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
                if(up_flag==1){
                    platform_num++;
                    sprintf (text, "          %d         ",platform_num);
                    LCD_DisplayStringLine(Line4, (uint8_t*)text);
                    if(platform_num==go_up[up_cnt1]){
                        LCD_Shanshuo();
                        LED_Show(led[platform_num-1], 0);
                        up_cnt1++;
                        door_open();
                        wait();
                        door_close();
                    }
                    if(up_cnt1==up_cnt){
                        memset(go_up, 0, 3);
                        up_flag=0;
                        up_cnt1=0;
                        up_cnt=0;
                    }
                }
                if(down_flag==1){
                    platform_num--;
                    sprintf (text, "          %d         ",platform_num);
                    LCD_DisplayStringLine(Line4, (uint8_t*)text);
                    if(platform_num==go_down[down_cnt1]){
                        LCD_Shanshuo();
                        LED_Show(led[platform_num-1], 0);
                        down_cnt1++;
                        door_open();
                        wait();
                        door_close();
                    }
                    if(down_cnt1==down_cnt){
                        memset(go_down, 0, 3);
                        down_flag=0;
                        down_cnt=0;
                        down_cnt1=0;
                    }
                }
                if(up_cnt1==0 && down_cnt1==0){
                    HAL_TIM_Base_Stop_IT(&htim2);
                    platform_state=0;
                    break;
                }
                TIM2->CNT=0;
                time=0;
                sec_6=0;
            }
        }
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        sec_1=1;
        time++;
        if(time==6){
            sec_6=1;
        }
    }
    if(htim->Instance==TIM3){
        sec_2=1;
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
