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
#include "math.h"
#include "string.h"
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
uint8_t cnbr_cnt, vnbr_cnt, idle_cnt=8, idx=0;  // cnbr类车数量，vnbr类车数量， 空闲车位, 结构体的索引
char car_type[5]; char car_id[5];
int year, month, day, hour, minute, second, park_time;
double cnbr_pay=3.5, vnbr_pay=2.0, pay=0.00;  // 两类车的停车费
uint8_t setting=0, tim17_flag=0;  // 标志是否进入设置页面, PA7输出PWM的标志
uint8_t key_val, key_up, key_down, key_last_val;
char send_buff[255], rec_buff[255];
uint8_t rec_data, send_data, count=0, rec_flag=0;
struct car{
    char type[5];
    char id[5];
    int year;
    int month;
    int day;
    int h;
    int m;
    int s;
}car[8];
// 每月天数表（非闰年）
const int days_per_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_Proc(void);
void KEY_Proc(void);
uint8_t KEY_Scan(void);
void LED_Proc(void);
void USART_Proc(void);
int findcar(char* type, char* id, int n);  // 确定该车是否已经进入车库
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
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    // LCD
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    // TIM
    TIM17->CCR1=20;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_TIM_Base_Start(&htim3);
    // USART1
    HAL_UART_Receive_IT(&huart1, &rec_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      LCD_Proc();
      KEY_Proc();
      USART_Proc();
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
    if(setting==0){
        sprintf(text, "       Data         ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "   CNBR:%01d           ", cnbr_cnt);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "   VNBR:%01d           ", vnbr_cnt);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
        sprintf(text, "   IDLE:%01d           ", idle_cnt);
        LCD_DisplayStringLine(Line7, (uint8_t*)text);
    }else{
        sprintf(text, "       Para         ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        sprintf(text, "   CNBR:%.2f", cnbr_pay);
        LCD_DisplayStringLine(Line3, (uint8_t*)text);
        sprintf(text, "   VNBR:%.2f", vnbr_pay);
        LCD_DisplayStringLine(Line5, (uint8_t*)text);
    }
    
}
void KEY_Proc(void){
    key_val=KEY_Scan();
    key_down=key_val&(key_val^key_last_val);
    key_up=~key_val&(key_val^key_last_val);
    if(key_val==1 && key_last_val==0){
        setting ^= 1;
        LCD_Clear(Black);
    }
    if(setting == 1 && key_val==2 && key_last_val==0){
        cnbr_pay += 0.5;
        vnbr_pay += 0.5;
    }
    if(setting == 1 && key_val==3 && key_last_val==0){
        cnbr_pay -= 0.5;
        if(cnbr_pay<0) cnbr_pay=0;
        vnbr_pay -= 0.5;
        if(vnbr_pay<0) vnbr_pay=0;
    }
    if(key_val==4 && key_last_val==0){
        tim17_flag ^= 1;
        if(tim17_flag==0){
            HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        }else{
            TIM17->CCR1=20;
            HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
        }
    }
    key_last_val=key_val;
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
    idle_cnt>0? LED_Show(0x01, 1) : LED_Show(0x01, 0);
    tim17_flag==0? LED_Show(0x02, 0) : LED_Show(0x02, 1);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        TIM3->CNT=0;
        rec_flag=1;
        rec_buff[count]=rec_data;
		count++;
		HAL_UART_Receive_IT(huart, &rec_data, 1);
    }
}
// -1表示不在车库，-2表示存在同一辆ID的车类型不同
int findcar(char* type, char* id, int n){
    int flag=-1;
    for(int i = 0;i < n;i++){
        if(strcmp(car[i].type, type)==0 && strcmp(car[i].id, id)==0){
            flag=i;
        }else if(strcmp(car[i].id, id)==0 && strcmp(car[i].type, type)!=0){
            flag=-2;
        }
    }
    return flag;
}
// 辅助函数：判断闰年
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
// 计算从公元元年到指定日期的总天数
unsigned long calculate_days(int year, int month, int day) {
    unsigned long days = day;
    for (int y = 1; y < year+2000; y++) {
        days += (365 + is_leap_year(y));
    }
    for (int m = 1; m < month; m++) {
        days += days_per_month[m-1];
        if (m == 2 && is_leap_year(year)) days += 1;
    }
    return days;
}
// 新增时间差计算函数
long int calculate_duration(int start_year, int start_month, int start_day, int start_h, int start_m, int start_s,
                                 int end_year, int end_month, int end_day, int end_h, int end_m, int end_s) {
    // 计算开始和结束时间的天数差
    unsigned long start_days = calculate_days(start_year, start_month, start_day);
    unsigned long end_days = calculate_days(end_year, end_month, end_day);
    unsigned long day_diff = 0;
    int ll = end_days - start_days;
    if(ll < 0){
        return -1;
    }else{
        day_diff = end_days - start_days;
    }
    if((end_h - start_h < 0) || (end_m - start_m < 0) || (end_s - start_s < 0)){
        return -1;
    }
    // 计算秒数差
    int time_diff_seconds = (end_h - start_h) * 3600 +
                          (end_m - start_m) * 60 +
                          (end_s - start_s);
    return (day_diff * 86400 + time_diff_seconds);
}
uint8_t checkCmd(char* str, uint8_t cnt){
    if(cnt!=22){
        return 0;
    }
    if((str[0] == 'C' || str[0] == 'V') && str[1] == 'N' && str[2] == 'B' && str[3] == 'R' && str[4] == ':' && str[9] == ':'){
		uint8_t i;
		for(i = 10; i < count; i++){
			if(str[i] > '9' || str[i] < '0')
				return 0;
		}
	}else{
        return 0;
    }
	return 1;
}
void USART_Proc(void){
    if(rec_flag){
        if(TIM3->CNT>15){
            if(checkCmd(rec_buff, count)==0){
                sprintf(send_buff, "Error\r\n");
                HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
            }else{
                sscanf(rec_buff, "%4s:%4s:%2d%2d%2d%2d%2d%2d", car_type, car_id, &year, &month, &day, &hour, &minute, &second);
                if(year > 99 || month > 12 || day > days_per_month[month-1] || hour > 23 || minute > 59 || second > 59){
                    sprintf(send_buff, "Error\r\n");
                    HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
                    goto restart;
                }
                int flag=findcar(car_type, car_id, cnbr_cnt+vnbr_cnt);
                if(flag==-1 && idx<8){
                    if(strcmp(car_type, "CNBR")==0){
                        cnbr_cnt++;
                        idle_cnt--;
                    }else if(strcmp(car_type, "VNBR")==0){
                        vnbr_cnt++;
                        idle_cnt--;
                    }
                    strcpy(car[idx].type,car_type);strcpy(car[idx].id,car_id);
                    car[idx].year=year;car[idx].month=month;car[idx].day=day;
                    car[idx].h=hour;car[idx].m=minute;car[idx].s=second;
                    idx++;
                }else if(flag != -1 && flag != -2){
                    long int time = calculate_duration(car[flag].year, car[flag].month, car[flag].day, car[flag].h, car[flag].m, car[flag].s,
                    year, month, day, hour, minute, second);
                    if(time==-1){
                        sprintf(send_buff, "Error\r\n");
                        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
                    }else{
                        park_time = ceil(1.0 * time / 3600);
                        if(strcmp(car_type, "CNBR")==0){
                            pay=park_time*cnbr_pay;
                            idle_cnt++;
                            cnbr_cnt--;
                        }else if(strcmp(car_type, "VNBR")==0){
                            pay=park_time*vnbr_pay;
                            idle_cnt++;
                            vnbr_cnt--;
                        }
                        sprintf(send_buff, "%4s:%4s:%d:%.2f\r\n", car[flag].type, car[flag].id, park_time, pay);
                        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
                        for(int i = flag; i < idx; i++) {
                            car[i] = car[i + 1]; // 直接通过结构体拷贝
                        }
                        idx--;
                    }
                }else if(flag == -2){
                    sprintf(send_buff, "Error\r\n");
                    HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
                }
            }
            restart:
            rec_flag=0;
            memset(rec_buff, 0, sizeof(rec_buff));
            count=0;
            // 重新启动接收中断（防御性编程）
            HAL_UART_Receive_IT(&huart1, &rec_data, 1);
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
