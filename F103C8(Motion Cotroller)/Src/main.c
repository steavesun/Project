/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 *
 * 개발자 : OH SUN CHUL
 * 컴파일러 : Atollic TrueStuio
 * 프로세서 : STM32F103C8T6
 *
 * 포트폴리오에 기재된 RC Car의 Firmware에 대한 Source Code입니다.
 *
 *
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <adc.h>
#include <gpio.h>
#include <math.h>
#include <sysclk.h>
#include <tim.h>
#include <uart.h>
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)

#define low 0
#define high 1
#define LOW 0
#define HIGH 1

#define ON 1
#define OFF 0
#define on 1
#define off 0




struct UART1 {

	uint8_t rxcnt;
	uint8_t txdata;
	uint8_t rxdata;
	char rxbuf[256];
	char txbuf[256];

}uart1;

struct UART3 {

	uint8_t rxcnt;
	uint8_t rxdata;
	char rxbuf[1024];
	char txbuf[256];


}uart3;



uint8_t adc_flag = ON;
uint8_t tim4_flag = LOW;

uint8_t PWM_CH1_FLAG = OFF;
uint8_t PWM_CH2_FLAG = OFF;
uint8_t PWM_CH3_FLAG = OFF;
uint8_t PWM_CH4_FLAG = OFF;


uint8_t tim2_flag = LOW;


uint32_t adc_resolution;

uint32_t pwm_resolution;
uint32_t ch1_pwm_resolution;
uint32_t ch2_pwm_resolution;
uint32_t ch3_pwm_resolution;
uint32_t ch4_pwm_resolution;

uint8_t   Dial_Mode_Flag = ON;

uint8_t TIM4_FLAG = ON;



// uint8_t start_flag = low;

char selection;

uint8_t Motor_Power_Flag = OFF;

uint8_t serial_flag = OFF;
uint8_t scroll_flag = ON;



GPIO_PinState SW1_STATE;
uint8_t SW1_CNT = 0;

GPIO_PinState SW2_STATE;
uint8_t SW2_CNT = 0;

GPIO_PinState SW3_STATE;
uint8_t SW3_CNT = 0;

GPIO_PinState SW4_STATE;
uint8_t SW4_CNT = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Motor_Power ( uint8_t );
void Motor_Dir(  uint8_t );



PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART2 and Loop until the end of transmission */

	if(serial_flag == ON)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
		return ch;

	}

}


void strclr(char *s)
{
	*s = '\0';
}



/* USER CODE END 0 */



/* USER CODE BEGIN 1 */
int main(void)
{


	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_TIM2_Init( 100  , 1000 , 0 ); // freq = 72*10^6 / 100 * 1000 = 720Hz

	// 32, 100 , 20,000Hz
	// 72 , 1000 , 1,000Hz
	// 144, 1000, 500H
	MX_TIM4_Init(7200,500,1000);
	MX_ADC1_Init();

	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);

	HAL_ADC_Start(&hadc1);
	HAL_ADCEx_Calibration_Start(&hadc1);


	MX_USART1_UART_Init(38400);      /* UART1 : USB TO TTL */
	MX_USART3_UART_Init(38400);      /* UART3 : BLUETOOTH  */


	HAL_UART_Receive_IT(&huart1,(uint8_t*)&uart1.rxdata, 1);
	HAL_UART_Receive_IT(&huart3,(uint8_t*)&uart3.rxdata, 1);

	HAL_TIM_Base_Start(&htim2);
	Motor_Dir(1);


	while (1)
	{



	}

}

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	/* UART1 ---> 시리얼 통신  */

	USART_TypeDef *UART = huart->Instance;
	if( UART == USART1 )
	{

		HAL_UART_Receive_IT(&huart1,(uint8_t*)&uart1.rxdata, 1);
		HAL_UART_Transmit(&huart1,(uint8_t*)&uart1.rxdata, 1,0xffff);

		uart1.rxbuf[uart1.rxcnt]  = uart1.rxdata;
		uart1.rxcnt++;

		if(uart1.rxdata == '\n') // 엔터를 입력시.
		{

			uart1.rxdata = '\0';
			uart1.rxcnt--;
			uart1.rxbuf[uart1.rxcnt] = '\0';


			/*   블루투스 설정시 활성화
			 *   sprintf(uart3.txbuf,"%s",uart1.rxbuf);
			 *   HAL_UART_Transmit(&huart3,(uint8_t*)uart3.txbuf, strlen(uart3.txbuf),10);
			 */



			printf("Slave : %s\n",uart1.rxbuf);
			if(strcmp(uart1.rxbuf,"ld1") == 0)
			{
				printf("Toggle LD1\n");
				HAL_GPIO_TogglePin(LD1_GPIO_PORT,LD1_PIN);
			}
			else if(strcmp(uart1.rxbuf,"ld2") == 0)
			{
				printf("Toggle LD2\n");
				HAL_GPIO_TogglePin(LD2_GPIO_PORT,LD2_PIN);
			}
			else if(strcmp(uart1.rxbuf,"ld3") == 0)
			{
				printf("Toggle LD3\n");
				HAL_GPIO_TogglePin(LD3_GPIO_PORT,LD3_PIN);
			}
			else if(strcmp(uart1.rxbuf,"ld4") == 0)
			{
				printf("Toggle LD4\n");
				HAL_GPIO_TogglePin(LD4_GPIO_PORT,LD4_PIN);
			}
			else if ( strcmp(uart1.rxbuf,"tim4") == 0 )
			{


				if (TIM4_FLAG == OFF)
				{
					printf("--- TIM4 : ACTIVATED ---\n");
					HAL_TIM_Base_Start(&htim4);
					HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
					TIM4_FLAG = ON;

				}
				else if ( TIM4_FLAG == ON)
				{

					printf("--- TIM4 : INACTIVATED ---\n");
					HAL_TIM_Base_Stop(&htim4);
					HAL_TIM_OC_Stop_IT(&htim4, TIM_CHANNEL_1);
					TIM4_FLAG = OFF;
				}

			}
			else if ( strcmp(uart1.rxbuf,"UART") == 0)
			{

				if(serial_flag == OFF)
				{
					serial_flag = ON;
					printf("--- UART : ACTIVATED ---\n");
				}
				else if (serial_flag == ON)
				{
					printf("--- UART : INACTIVATED ---\n");
					serial_flag = OFF;
				}

			}
			strclr(uart1.rxbuf);
			// strclr(uart3.txbuf);
			uart1.rxcnt = 0;

		}
		else if ( uart1.rxdata == '\b')
		{
			uart1.rxdata = '\0';
			HAL_UART_Transmit(&huart1, (uint8_t *)" ", 1, 0xFFFF);
			HAL_UART_Transmit(&huart1, (uint8_t *)"\b", 1, 0xFFFF);
			uart1.rxcnt--;
			uart1.rxbuf[uart1.rxcnt] = '\0';
			uart1.rxcnt--;
			uart1.rxbuf[uart1.rxcnt] = '\0';
		}

	}

	/* UART3 ---> 블루투스 */
	/* PC로부터 수신된 블루투스 데이터 처리하여 모터 제어 */
	else if( UART == USART3 )
	{

		HAL_UART_Receive_IT(&huart3,(uint8_t*)&uart3.rxdata, 1);
		//  uart1.txdata = uart3.rxdata;
		//  HAL_UART_Transmit(&huart1,(uint8_t*)&uart1.txdata, 1,0xffff);

		uart3.rxbuf[uart3.rxcnt]  = uart3.rxdata;
		uart3.rxcnt++;


		if(uart3.rxdata == '\n')
		{

			uart3.rxdata = '\0';
			uart3.rxcnt--;
			uart3.rxbuf[uart3.rxcnt] = '\0';

			//	printf("블루투스 : %s\n",uart3.rxbuf);

			if(strcmp(uart3.rxbuf,"Key Mode = ON") == 0) // PC로 부터 Key Mode가 체크되어야 자동차가 W,A,S,D키로 구동됨.
			{
				//	 printf("--- Key Mode = ON ---\n");
				Motor_Power(ON);
				Motor_Dir(7);

			}

			else if(strcmp(uart3.rxbuf,"Key Mode = OFF") == 0)
			{
				//	  printf("--- Key Mode = OFF ---\n");
				Motor_Power(OFF);
				Motor_Dir(1);

			}
			/* 키보드의 Q,W,E,A,S,D,Z,X,C버튼으로 부터 자동차를 제어한다 */
			else if(strcmp(uart3.rxbuf,"W") == 0 ) // 키보드에서 W누를시 전진.
			{

				Motor_Dir(1);

				if( Motor_Power_Flag == OFF)
				{

					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"S") == 0 ) // 키보드에서 S누를시 정지.
			{

				//	  	printf("Motor Direction : HOLD\n");
				Motor_Dir(7);
				Motor_Power(OFF);


			}

			else if(strcmp(uart3.rxbuf,"X") == 0 )
			{

				//	  	printf("Motor Direction : BACKWARD\n");
				Motor_Dir(4);
				if( Motor_Power_Flag == OFF)
				{

					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"Q") == 0 )
			{

				//	  	printf("Motor Direction : LEFT - FORWARD\n");
				Motor_Dir(2);
				if( Motor_Power_Flag == OFF )
				{

					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"E") == 0 ) // E버튼 누르면 우측 전진
			{

				//	  	printf("Motor Direction : RIGHT - FORWARD\n");
				Motor_Dir(6);

				if( Motor_Power_Flag == OFF )
				{
					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"Z") == 0 )
			{
				//	  	printf("Motor Direction : LEFT - BACKWARD\n");
				Motor_Dir(3);
				if( Motor_Power_Flag == OFF)
				{

					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"C") == 0 )
			{

				//	  	printf("Motor Direction : RIGHT - BACWARD\n");
				Motor_Dir(5);
				if( Motor_Power_Flag == OFF )
				{

					Motor_Power(ON);

				}

			}
			else if(strcmp(uart3.rxbuf,"Dial Mode = OFF") == 0)
			{

				//   printf("--- Dial Mode : OFF ---\n");
				HAL_TIM_OC_Stop_IT(&htim4, TIM_CHANNEL_1);
				Motor_Dir(7);
				Motor_Power(OFF);
				Dial_Mode_Flag = OFF;

			}
			else if(strcmp(uart3.rxbuf,"Dial Mode = ON") == 0)
			{

				//  printf("--- Dial Mode : ON ---\n");
				HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
				Motor_Dir(1);
				Dial_Mode_Flag = ON;

			}

			else if ( strcmp(uart3.rxbuf,"LD1") == 0 )
			{

				HAL_GPIO_TogglePin(LD1_GPIO_PORT,LD1_PIN);
				//  printf("--- LD1 : Toggle ---\n");

			}
			else if ( strcmp(uart3.rxbuf,"LD2") == 0 )
			{

				HAL_GPIO_TogglePin(LD2_GPIO_PORT,LD2_PIN);
				//	  printf("--- LD2 : Toggle ---\n");

			}
			else if ( strcmp(uart3.rxbuf,"LD3") == 0 )
			{

				HAL_GPIO_TogglePin(LD3_GPIO_PORT,LD3_PIN);
				//	  printf("--- LD3 : Toggle ---\n");

			}
			else if ( strcmp(uart3.rxbuf,"LD4") == 0 )
			{

				HAL_GPIO_TogglePin(LD4_GPIO_PORT,LD4_PIN);
				//  printf("--- LD4 : Toggle ---\n");

			}
			if(scroll_flag == ON)
			{

				if(selection == '1')
				{

					selection = '\0';
					ch1_pwm_resolution =  atoi(uart3.rxbuf);
					analogWrite_ch1(ch1_pwm_resolution);
					//	  printf("CH1 PWM Resolution : %d\n",ch1_pwm_resolution);

				}
				else if( selection ==  '2')
				{
					selection = '\0';
					ch2_pwm_resolution =  atoi(uart3.rxbuf);
					analogWrite_ch2(ch2_pwm_resolution);
					//	  printf("CH2 PWM Resolution : %d\n",ch2_pwm_resolution);

				}
				else if ( selection == '3')
				{
					selection = '\0';
					ch3_pwm_resolution =  atoi(uart3.rxbuf);
					analogWrite_ch3(ch3_pwm_resolution);
					//	  printf("CH3 PWM Resolution : %d\n",ch3_pwm_resolution);

				}
				else if ( selection == '4')
				{

					selection = '\0';
					ch4_pwm_resolution =  atoi(uart3.rxbuf);
					analogWrite_ch4(ch4_pwm_resolution);
					//	  printf("CH4 PWM Resolution : %d\n",ch4_pwm_resolution);
				}

				else if ( selection == '5') // M의 스크롤바 데이터 처리
				{

					selection = '\0';
					pwm_resolution =  atoi(uart3.rxbuf);
					analogWrite(pwm_resolution);
					//   printf("PWM Resolution : %d\n",pwm_resolution);

				}

				scroll_flag = OFF;

			}

			strclr(uart3.rxbuf);
			//  strclr(uart1.txbuf);
			uart3.rxcnt = 0;

		}
		else if ( uart3.rxdata == '\b')
		{
			uart3.rxdata = '\0';
			HAL_UART_Transmit(&huart1, (uint8_t *)" ", 1, 0xFFFF);
			HAL_UART_Transmit(&huart1, (uint8_t *)"\b", 1, 0xFFFF);
			uart3.rxcnt--;
			uart3.rxbuf[uart3.rxcnt] = '\0';
			uart3.rxcnt--;
			uart3.rxbuf[uart3.rxcnt] = '\0';

		}
		else if( uart3.rxdata == ':') //
		{

			uart3.rxcnt--;
			uart3.rxbuf[uart3.rxcnt] = '\0';
			uart3.rxcnt--;
			selection = uart3.rxbuf[uart3.rxcnt];
			uart3.rxcnt = 0;
			scroll_flag = ON;
		}

	}

}



void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{




	if( htim->Instance == TIM4  && htim ->Channel  ==  HAL_TIM_ACTIVE_CHANNEL_1  )
	{



		// adc값을 바탕으로 pwm 듀티비 조절한다.

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 0xffff);
		adc_resolution  = HAL_ADC_GetValue(&hadc1);

		pwm_resolution =   floor( adc_resolution *  MAX_PWM_RESOLUTION/MAX_ADC_RESOLUTION ); // 0 ~ MAX_PWM_RESOLUTION의 듀티비 스텝.

		analogWrite(pwm_resolution);

		//  printf("ADC Resolution : %d\n",adc_resolution);
		//   printf("PWM Resolution : %d\n",pwm_resolution);


		HAL_ADC_Stop(&hadc1);





		SW1_STATE = HAL_GPIO_ReadPin(SW1_GPIO_PORT,SW1_PIN);

		SW2_STATE =  HAL_GPIO_ReadPin(SW2_GPIO_PORT,SW2_PIN);

		SW3_STATE = HAL_GPIO_ReadPin(SW3_GPIO_PORT,SW3_PIN);

		SW4_STATE = HAL_GPIO_ReadPin(SW4_GPIO_PORT,SW4_PIN);


		//  printf("SW1 = %d\n",SW1_STATE);
		//  printf("SW2 = %d\n",SW2_STATE);
		//  printf("SW3 = %d\n",SW3_STATE);
		//  printf("SW4 = %d\n",SW4_STATE);


		if( SW1_STATE == 0 )
		{

			if(SW1_CNT == 0)
			{

				SW1_CNT++;
				//	 printf("--- SW1 Pushed ---\n");
				HAL_GPIO_TogglePin(LD1_GPIO_PORT,LD1_PIN);

				if(PWM_CH1_FLAG == 0) //
				{

					//	 printf("--- PWM CH1 Start ---\n");
					//	 HAL_TIM_Base_Start(&htim2);
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
					PWM_CH1_FLAG = 1;
					Motor_Dir(1);


				}

				else if(PWM_CH1_FLAG == 1) //
				{

					// printf("--- PWM CH1 Stop ---\n");

					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
					PWM_CH1_FLAG = 0;
					//	 Motor_Dir(7);


				}

			}

		}
		else if ( SW1_STATE == 1)
		{

			if(SW1_CNT != 0)
			{
				SW1_CNT = 0;
			}
		}

		if( SW2_STATE == 0 )
		{
			if(SW2_CNT == 0)
			{

				SW2_CNT++;
				// printf("--- SW2 Pushed ---\n");
				HAL_GPIO_TogglePin(LD2_GPIO_PORT,LD2_PIN);

				if(PWM_CH2_FLAG == 0)
				{
					//	 printf("--- PWM CH2 Start ---\n");
					//	 HAL_TIM_Base_Start(&htim2);
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
					PWM_CH2_FLAG = 1;
					Motor_Dir(1);

				}

				else if(PWM_CH2_FLAG == 1)
				{
					//	 printf("--- PWM CH2 Stop ---\n");
					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
					PWM_CH2_FLAG = 0;
					//	 Motor_Dir(7);

				}

			}


		}
		else if ( SW2_STATE == 1)
		{

			if(SW2_CNT != 0)
			{
				SW2_CNT = 0;

			}
		}
		if( SW3_STATE == 0 )
		{

			if(SW3_CNT == 0)
			{

				SW3_CNT++;
				//  printf("--- SW3 Pushed ---\n");
				HAL_GPIO_TogglePin(LD3_GPIO_PORT,LD3_PIN);

				if(PWM_CH3_FLAG == 0)
				{

					// 	 printf("--- PWM CH3 Start ---\n");
					//	 HAL_TIM_Base_Start(&htim2);
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
					PWM_CH3_FLAG = 1;
					Motor_Dir(1);


				}

				else if(PWM_CH3_FLAG == 1)
				{

					//	 printf("--- PWM CH3 Stop ---\n");
					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
					PWM_CH3_FLAG = 0;
					//	 Motor_Dir(7);

				}

			}

		}
		else if ( SW3_STATE == 1)
		{

			if(SW3_CNT != 0)
			{
				SW3_CNT = 0;
			}
		}
		if( SW4_STATE == 0 )
		{

			if(SW4_CNT == 0)
			{

				SW4_CNT++;
				//	 printf("--- SW4 Pushed ---\n");
				HAL_GPIO_TogglePin(LD4_GPIO_PORT,LD4_PIN);

				if(PWM_CH4_FLAG == 0) // not PWM CH1 OPERATED.
				{

					//	 printf("--- PWM CH4 Start ---\n");
					//	 HAL_TIM_Base_Start(&htim2);
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
					PWM_CH4_FLAG = 1;
					Motor_Dir(1);


				}

				else if(PWM_CH4_FLAG == 1)
				{

					//	 printf("--- PWM CH4 Stop ---\n");
					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
					PWM_CH4_FLAG = 0;
					//	 Motor_Dir(7);
				}
			}

		}
		else if ( SW4_STATE == 1)
		{

			if(SW4_CNT != 0)
			{
				SW4_CNT = 0;

			}
		}

	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


	/*
	if(GPIO_Pin == SW1_PIN )
	{

		HAL_GPIO_TogglePin(LD1_GPIO_PORT,LD1_PIN);

		if(Dial_Mode_Flag == ON)
		{

			if(PWM_CH1_Flag == OFF )
			{
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
				PWM_CH1_Flag = ON;
			}
			else if ( PWM_CH1_Flag == ON )
			{
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
				PWM_CH1_Flag = OFF;
			}
		}
	}
	else	if(GPIO_Pin == SW2_PIN )
	{

		HAL_GPIO_TogglePin(LD2_GPIO_PORT,LD2_PIN);
		if(Dial_Mode_Flag == ON)
		{

			if(PWM_CH2_Flag == OFF )
			{
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
				PWM_CH2_Flag = ON;
			}
			else if ( PWM_CH2_Flag == ON )
			{
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
				PWM_CH2_Flag = OFF;
			}
		}

	}
	else	if(GPIO_Pin == SW3_PIN)
	{

		HAL_GPIO_TogglePin(LD3_GPIO_PORT,LD3_PIN);
		if(Dial_Mode_Flag == ON)
		{


			if(PWM_CH3_Flag == OFF )
			{
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
				PWM_CH3_Flag = ON;
			}
			else if ( PWM_CH3_Flag == ON )
			{
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
			 PWM_CH3_Flag = OFF;

			}

		}

	}
	else	if(GPIO_Pin == SW4_PIN)
	{

		HAL_GPIO_TogglePin(LD4_GPIO_PORT,LD4_PIN);
		if(Dial_Mode_Flag == ON)
		{

			if(PWM_CH4_Flag == OFF )
			{
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
				PWM_CH4_Flag = ON;
			}
			else if ( PWM_CH4_Flag == ON )
			{
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
				PWM_CH4_Flag = OFF;
			}
		}

	}
	 */

}
void Motor_Power ( uint8_t state)
{

	switch(state)
	{
			case ON :
			{

				// 	HAL_TIM_Base_Start(&htim2);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
				Motor_Power_Flag = ON;
				break;

			}
			case OFF :
			{


				//	HAL_TIM_Base_Stop(&htim2);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
		        Motor_Power_Flag = OFF;
			    break;

			}
	}

}
void Motor_Dir(uint8_t dir)
{

	switch(dir)
	{
			case 1 : // forward
			{

				IN0(0); IN1(1);  IN6(0); IN7(1);
				IN2(0); IN3(1);  IN4(0); IN5(1);


				break;
			}
			case 2 :
			{

				IN0(0); IN1(0);  IN6(0); IN7(1);
				IN2(0); IN3(0);  IN4(0); IN5(1);

				break;
			}
			case 3 :
			{
				IN0(0); IN1(0);  IN6(1); IN7(0);
				IN2(0); IN3(0);  IN4(1); IN5(0);

				break;
			}
			case 4 : // backward
			{

				IN0(1); IN1(0);  IN6(1); IN7(0);
				IN2(1); IN3(0);  IN4(1); IN5(0);

				break;
			}
			case 5 :
			{
				IN0(1); IN1(0);  IN6(0); IN7(0);
				IN2(1); IN3(0);  IN4(0); IN5(0);

				break;
			}
			case 6 :
			{
				IN0(0); IN1(1);  IN6(0); IN7(0);
				IN2(0); IN3(1);  IN4(0); IN5(0);

				break;
			}
			case 7 :
			{
				IN0(0); IN1(0);  IN6(0); IN7(0);
				IN2(0); IN3(0);  IN4(0); IN5(0);

				break;
			}
	}



}


/* USER CODE END 2 */


