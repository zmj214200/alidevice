/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdio.h>

#include <time.h>

#include <stdlib.h>
#include "Iot_config.h"

#include "Hmacsha1.h"

#include "https_download.h"

#include "mqtt_connect.h"

#include "GPS.h"

#include "common.h"

#include "modbus_master.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern  	int mbedtls_timing_alarmed;
extern  	uint32_t alarm_time;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;
TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

uint8_t huart1_buf;
uint8_t huart1_rev_flag=0;

uint8_t huart2_buf;
uint8_t huart2_rev_flag=0;

uint8_t huart3_buf;
uint8_t huart3_rev_flag=0;

uint8_t huart4_buf;
uint8_t huart4_rev_flag=0;

uint8_t huart5_buf;
uint8_t huart5_rev_flag=0;

uint8_t huart6_buf;
uint8_t huart6_rev_flag=0;


uint8_t Delay_break=0;
uint8_t Delay_break_net=0;

uint8_t Lan_rev_flag=0;

//////////////////////////////////
uint8_t    CSQ_Status =0;
uint8_t    Cmnbiot_ready =0;
uint8_t    CSQ_Strength;

char       Lan_rev_data[SIM_REV_BUF_MAX] ;
uint32_t   Lan_rev_lenth;

char       Mqtt_rev_data[2048];
uint32_t   Mqtt_rev_length;

char       Mqtt_rev_data_s[1024];
uint32_t   Mqtt_rev_s_length;

uint8_t    ServerIP_ready =0;
char       ServerIP[20];
uint8_t    ServerIP_length;

uint8_t		 ATE_STATUS =0;						 //IOT 模块回显状态
uint8_t    AT_ready =0;              //AT 有正确返回
uint8_t    Sim_ready =0;             // SIM卡 REDY
uint8_t    CPIN_ready =0; 
uint8_t    Send_OK=0;

uint8_t    Cline_Status_1;
uint8_t    Cline_Status_2;

uint8_t  	 IOT_Step;              //记录网络模块运行状态
uint8_t    SIM_Init_Flag=0;          //SIM 初始化 标志
uint8_t    SIM_Init_OK=0;            //SIM 初始化中


uint8_t    NetOpen_Status_1 =0;
uint8_t    TCP_numb =0;
uint8_t    NetOpen_Status_2 =0;

uint8_t   clinent_server_error =0;
uint8_t   CMDR_OK =0;

////////////////////////////////////////////
uint8_t   reset_flag =0;

uint8_t 	Rev_flag=0;

uint8_t   MQTT_Client_OK =0;
uint8_t   NTP_return_ERR =0;
/////////////////////////////////
uint8_t   runticks =1;

///////////////////////////////////////////////////
uint8_t   end_cmd[2]={0x0D,0x0A};

uint32_t  SIM_UART_Timeout=0;  //IOT 模块接收超时计数        			 
uint8_t   Wait_times =1;            //超时时间
uint8_t   SIM_DATA_BUF[SIM_REV_BUF_MAX];  		// IOT 数据接收BUF
uint32_t  SIM_Buf_length =0;          				//  接收数据长度

uint8_t    RETURN_OK=0;          //

char SUB_Topic_data[1024];
char SUB_Topic_data_length;

uint8_t Sub_topic_ready=0;

  RTC_TimeTypeDef  Times  ;
  RTC_DateTypeDef  Dates  ;
  

uint8_t  New_version_ready=0;
uint8_t  Start_upgr=0;

/**/
uint8_t QMTOPEN_FLAG =0;
uint8_t QMTCONN_Flag =0;
extern uint32_t App_Write_Offset;
uint8_t Powon_flag=0;

/**/
uint8_t LED_Numb_Flag=0;
uint8_t LED_Status=0;
uint8_t LED_Flash_flag;

/**/
uint8_t 	 NTP_Time_ready=0;  
time_t     Sys_times;
time_t		 server_send_time;
time_t     server_rev_time;
struct tm  local_time;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
static void MX_TIM12_Init(void);
static void MX_TIM13_Init(void);
static void MX_TIM14_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM8_Init(void);

/* USER CODE BEGIN PFP */
uint8_t  systicks =1;
/* USER CODE END PFP */
extern uint8_t net_init;
/*Mqtt*/
Boot_pram  *Boot_pram_data;
App_Pram	 *App_Pram_data;
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t   Cheart;

void feed_dog()
{

	HAL_GPIO_WritePin(MCU_WDI_GPIO_Port, MCU_WDI_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(MCU_WDI_GPIO_Port, MCU_WDI_Pin, GPIO_PIN_RESET);
}

void savenv()
{

		App_Pram_data->Write_Flag =0;
		while(App_Pram_data->Write_Flag!=0x55)
		{
			App_Pram_data->Write_Flag =0x55;
			Flash_write(App_pram_addr ,(uint8_t *)App_Pram_data, sizeof(App_Pram),11,1);
			App_Pram_data->Write_Flag =0;
			FLASH_Read(App_pram_addr,(uint8_t *)App_Pram_data,sizeof(App_Pram));								
		}
}
void savebootnv()
{
		feed_dog();
		Boot_pram_data->Write_Flag =0;
		while(Boot_pram_data->Write_Flag!=0x55)
		{
			Boot_pram_data->Write_Flag =0x55;
			Flash_write(Boot_pram_addr ,(uint8_t *)Boot_pram_data, sizeof(Boot_pram),10,1);
			Boot_pram_data->Write_Flag =0;
		  FLASH_Read(Boot_pram_addr,(uint8_t *)Boot_pram_data,sizeof(Boot_pram));						
		}
}

uint16_t HextoString(char *datain,uint16_t length,char *dataout)
{
	uint16_t i,j;
	if((length%2)==0)
	{
		for(i=0;i<length/2;i++)
		{
				if(datain[2*i]<0x40)
				{
						if(datain[2*i+1]<0x40)
						{
						dataout[i]=(datain[2*i]-0x30)*0x10+(datain[2*i+1]-0x30);
						}
						else if(datain[2*i+1]<0x47)
						{
							dataout[i]=(datain[2*i]-0x30)*0x10+(datain[2*i+1]-0x41+10);
						}
						else
						{
							dataout[i]=(datain[2*i]-0x30)*0x10+(datain[2*i+1]-0x61+10);
						}
					}
				else if(datain[2*i]<0x47)
					{
						if(datain[2*i+1]<0x40)
						{
							dataout[i]=(datain[2*i]-0x41+10)*0x10+(datain[2*i+1]-0x30);
						}
						else if(datain[2*i+1]<0x47)
						{
								dataout[i]=(datain[2*i]-0x41+10)*0x10+(datain[2*i+1]-0x41+10);
						}
						else
						{
								dataout[i]=(datain[2*i]-0x41+10)*0x10+(datain[2*i+1]-0x61+10);
						}
					}
					else
					{
						if(datain[2*i+1]<0x40)
						{
							dataout[i]=(datain[2*i]-0x61+10)*0x10+(datain[2*i+1]-0x30);
						}
						else if(datain[2*i+1]<0x47)
						{
								dataout[i]=(datain[2*i]-0x61+10)*0x10+(datain[2*i+1]-0x41+10);
						}
						else
						{
								dataout[i]=(datain[2*i]-0x61+10)*0x10+(datain[2*i+1]-0x61+10);
						}
					}
		
		}
		return length/2;
	}
	
}



/*
rev data process

*/
void GSM_Status_Check(uint8_t *data,uint32_t length)
{
 	uint32_t   i,j,n,m;
	uint32_t   rev_id;
//	uint32_t   rev_numb;
	uint32_t   rev_int_temp;
	uint32_t   local_numb[5];
	uint8_t    package_numb;
	uint32_t   rev_length_temp;
	char 			 Topic_temp_data[256];
	char       temp_value[20];
	if(SIM_Init_Flag)
	{
		if(strstr((char*)data,"OK")!=NULL)
		{
					 ATE_STATUS = 1;
					 AT_ready = 1;
			     IOT_Step =1;
					 Delay_break =1;
		}

		if(strstr((char*)data,"+CPIN:")!=NULL)
		{
			 if(strstr((char*)data,"READY")!=NULL)
				{
								Sim_ready =1;
								CPIN_ready =1;
								IOT_Step =1;
								Delay_break =1;
								LED_Numb_Flag = LED_Numb_Flag|0x10;
					}
			}
	}
	
	if(strstr((char*)data,"OK")!=NULL)
		{
			if(IOT_GSM) 
			{
				QMTOPEN_FLAG =1;	
				QMTCONN_Flag =1;
			}
			CMDR_OK =1;
			Delay_break=1;
			Send_OK =1;
			Delay_break=1;
			if(IOT_GSM) 
			{
				Cline_Status_1 =1;
			}
		}
		//+QMTOPEN: 0,-1
	if(strstr((char*)data,"+QMTOPEN:")!=NULL)
	{
			if(strstr((char*)data,"-1")!=NULL)
			{
			
			}
			else
			{
				QMTOPEN_FLAG =1;
			}
				
	}
	//+QMTSTAT: 0,1
	if(strstr((char*)data,"+QMTSTAT:")!=NULL)
	{
			 for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='Q')&& (*(data+i+2)=='M')&& (*(data+i+3)=='T')&& (*(data+i+4)=='S')&& (*(data+i+5)=='T')) 
					{
								if(*(data+i+12)=='1')
										QMTOPEN_FLAG =0;
					}
				}
	
	}

	if(strstr((char*)data,"+QMTCONN:")!=NULL)
	{
			QMTCONN_Flag =1;	
	}
	//+QIOPEN: 0,0
		if(strstr((char*)data,"+QIOPEN:")!=NULL)
	{  
		for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='Q')&& (*(data+i+2)=='I')&& (*(data+i+3)=='O')&& (*(data+i+4)=='P')&& (*(data+i+5)=='E')) 
					{
								if(*(data+i+11)=='0')
								{
										Delay_break_net =1;
										Cline_Status_1 =1;
								}
					}
				}
	}
	if(strstr((char*)data,"+IP:")!=NULL)	
	{
			Powon_flag =1;
	
	}
		
	if(strstr((char*)data,"+CSQ")!=NULL)
	{
		for(i=0;i<length;i++)
		{
			if((*(data+i)=='C')&& (*(data+i+1)=='S')&& (*(data+i+2)=='Q')&& (*(data+i+3)==':')) 
			{
						if(*(data+i+6)==',')
						{
							CSQ_Strength = *(data+i+5)-0x30;
							break;
						}
						else if(*(data+i+7)==',')
						{
							CSQ_Strength = (*(data+i+5)-0x30)*10+*(data+i+6)-0x30;
							break;
						}
						else
						{
							break;
						}
					 
			}
		}
		  if((CSQ_Strength>0)&&(CSQ_Strength<31))
			{
				CSQ_Status =1;
				Delay_break =1;
			}
		  printf("CSQ = %d \n",CSQ_Strength);
	}
	
		if(strstr((char*)data,"+CGCONTRDP:")!=NULL)   //联网状态
		{
				if(strstr((char*)data,"cmnbiot")!=NULL)
				{
						Cmnbiot_ready =1;
						Delay_break =1;
				
				}
		
		}
				
		if(strstr((char*)data,"+CDNSGIP:")!=NULL)   //DNS 解析IP
		{			
			
			  for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='C')&& (*(data+i+2)=='D')&& (*(data+i+3)=='N')&& (*(data+i+4)=='S')&& (*(data+i+5)=='G')) 
					{
							/*for(n=0;n<15;n++)
							{
									if(*(data+i+6+n)=='"')
									{
											memcpy(ServerIP,data+i+6,n);
										  ServerIP_length =n;
										  ServerIP_ready =1;
											Delay_break =1;
										  break;
									}
							}*/
									sscanf( (char*)data+i, "+CDNSGIP: 1,\"ota.iot-thing.cn-shanghai.aliyuncs.com\",\"%s\"\r\n",ServerIP);
									ServerIP_length =strlen((char*)ServerIP);
									
									if(ServerIP_length)
									{
										ServerIP[ServerIP_length-1]=0;
										ServerIP_ready=1;
									}
									break;
					}
				}
			

			Delay_break =1;
		}
		if(strstr((char*)data,"+CSOC:")!=NULL)   //联网状态
		{
			//判断是哪一路连接 CSOC：0 CSOC:1
				for(i=0;i<length;i++)
				{
						if((*(data+i)=='C')&&(*(data+i+1)=='S')&& (*(data+i+2)=='O')&& (*(data+i+3)=='C')&& (*(data+i+4)==':')) 
						{
								if((*(data+i+6))=='0')
								{
									NetOpen_Status_1 =1;
									printf("NetOpen_Status_1==1\r\n");
									TCP_numb = 0;
									Delay_break =1;
									break;
									}
								else if((*(data+i+6))=='1')
									{
										NetOpen_Status_2 =1;
										TCP_numb = 1;
										Delay_break =1;
										break;
									}
							}
					}
			}
		if(IOT_GSM) 
		{
				if(strstr((char*)data,"ERROR")!=NULL) 
				{
					if(NTP_return_ERR !=1)
					{
						clinent_server_error =1;
					}
				 
				}
		
		}

		
		if(strstr((char*)data,"+CSONMI: 0")!=NULL)
		{
	//		+CSONMI: 0,1024,616161616161616161616161616161616161616161
		/*	if(Rev_flag ==0)
			{
				package_numb =0;
				
				for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='C')&& (*(data+i+2)=='S')&& (*(data+i+3)=='O')&& (*(data+i+4)=='N')&& (*(data+i+5)=='M'))
					{
							local_numb[package_numb] =i;
							package_numb++;
					}
				}
					rev_length_temp=0;
					Lan_rev_lenth =0;
					for(j=0;j<package_numb;j++)
					{//,
						sscanf( (char*)data+local_numb[j], "+CSONMI: 0,%d,\r\n",&rev_length_temp);
						printf("rev_length_temp = %d \n\r",rev_length_temp);
						if(rev_length_temp>999)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+16+m);
							}
						}else	if(rev_length_temp>99)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+15+m);
							}
						}
						else if(rev_length_temp>9)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+14+m);
							}
						}
						else if(rev_length_temp>0)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+13+m);
							}
						}
						
						Lan_rev_lenth = Lan_rev_lenth+rev_length_temp;
					}
					printf("Lan_rev_lenth = %d \n\r",Lan_rev_lenth);
					if(Lan_rev_lenth)
					{
						Rev_flag=1;
						
					}
					printf("Rev_flag: %d\n\r",Rev_flag);
				}
			*/
			if(Rev_flag ==0)
			{
				package_numb =0;
				
				for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='C')&& (*(data+i+2)=='S')&& (*(data+i+3)=='O')&& (*(data+i+4)=='N')&& (*(data+i+5)=='M'))
					{
							local_numb[package_numb] =i;
							package_numb++;
					}
				}
				  rev_length_temp=0;
					Lan_rev_lenth =0;
					for(j=0;j<package_numb;j++)
					{
						sscanf( (char*)data+local_numb[j], "+CSONMI: 0,%d,%s\r\n",&rev_length_temp,Lan_rev_data+Lan_rev_lenth);
						printf("rev_length_temp = %d \n\r",rev_length_temp);
						Lan_rev_lenth = Lan_rev_lenth+rev_length_temp;
					}
					printf("Lan_rev_lenth = %d \n\r",Lan_rev_lenth);
					if(Lan_rev_lenth)
					{
						Rev_flag=1;
						
					}
					printf("Rev_flag: %d\n\r",Rev_flag);
			}
			
			}
		//+QIURC: "recv",0,10
//0123456789
		if(strstr((char*)data,"+QIURC:")!=NULL)
		{
					if(Rev_flag ==0)
			{
				package_numb =0;
				
				for(i=0;i<length;i++)
				{
					if((*(data+i)=='+')&& (*(data+i+1)=='Q')&& (*(data+i+2)=='I')&& (*(data+i+3)=='U')&& (*(data+i+4)=='R')&& (*(data+i+5)=='C'))
					{
							local_numb[package_numb] =i;
							package_numb++;
					}
				}
					rev_length_temp=0;
					Lan_rev_lenth =0;
					for(j=0;j<package_numb;j++)
					{//,
						sscanf( (char*)data+local_numb[j], "+QIURC: \"recv\",0,%d\r\n",&rev_length_temp);
						printf("rev_length_temp = %d \n\r",rev_length_temp);
						if(rev_length_temp>99)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+22+m);
							}
						}
						else if(rev_length_temp>9)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+21+m);
							}
						}
						else if(rev_length_temp>0)
						{
							for(m=0;m<rev_length_temp;m++)
							{
								Lan_rev_data[Lan_rev_lenth+m]=*(data+local_numb[j]+20+m);
							}
						}
						
						Lan_rev_lenth = Lan_rev_lenth+rev_length_temp;
					}
					printf("Lan_rev_lenth = %d \n\r",Lan_rev_lenth);
					if(Lan_rev_lenth)
					{
						Rev_flag=1;
						
					}
					printf("Rev_flag: %d\n\r",Rev_flag);
				
			}
		
		}
		if(strstr((char*)data,"+QMTRECV:")!=NULL)  //B20 rev Mqtt data
		{
		
		
		}
		
		 if (strstr((char*)data,"+CSOERR")!=NULL)
		  {
						for(i=0;i<length;i++)
						{
								if((*(data+i)=='+')&&(*(data+i+1)=='C')&& (*(data+i+2)=='S')&& (*(data+i+3)=='O')&& (*(data+i+4)=='E')&& (*(data+i+5)=='R')&& (*(data+i+6)=='R')) 
								{
										if(*(data+i+10)==',')
										{
													if((*(data+i+9))=='0')
													{   printf("\r\n%s \r\n",data);
															printf("+CSOERR  NetOpen_Status_1==0\r\n");
															NetOpen_Status_1 =0;
														  Cline_Status_1 =0;
														if(App_Pram_data->downloading ==1)
														{
															savenv();
															while(1); //等待重启
														}
														
															break;
													 }
													else if((*(data+i+9))=='1')
													{
														NetOpen_Status_2 =0;
														break;
													}
										}
								}
						}
						//printf("Transfer complete S\n");
					 // Download_OK=1;
		}
		if(strstr((char*)data,"+CMQPUB:")!=NULL)  //收到sub topic
		{
			if(strstr((char*)data,"/thing/service/property/set")!=NULL) 
			{
					for(i=0;i<length;i++)
				{
						if((*(data+i)=='s')&&(*(data+i+1)=='e')&& (*(data+i+2)=='t')&& (*(data+i+3)=='"')&& (*(data+i+4)==','))
						{
								if((*(data+i+13)==',')&&(*(data+i+14)=='"'))
								{
										Mqtt_rev_length = (*(data+i+11)-0x30)*10+(*(data+i+12)-0x30);
									
										if(*(data+i+14+Mqtt_rev_length+1 )=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+15+j);
													}
												
										}
										
									
								}
								else if((*(data+i+14)==',')&&(*(data+i+15)=='"'))
								{
									Mqtt_rev_length =(*(data+i+11)-0x30)*100 + (*(data+i+12)-0x30)*10+(*(data+i+13)-0x30);
									
									if(*(data+i+15+Mqtt_rev_length+1 )=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+16+j);
													}
												
										}
								}
						}
				}
				/*+CMQPUB: 0,"/sys/a1n7HqCJq4Y/3325262728EB/thing/service/
				/set",0,0,0,206,"7B226D6574686F64223A227468696E672E736572766963652E70726F70657274792E736574222C226964223A2231333237343933313339222C22706172616D73223A7B2243757272656E745F4C696D74223A337D2C2276657273696F6E223A22312E302E30227D"*/
				memset(Mqtt_rev_data_s,0,sizeof(Mqtt_rev_data_s));
				Mqtt_rev_s_length = HextoString(Mqtt_rev_data,Mqtt_rev_length,Mqtt_rev_data_s);
				
				memset(Mqtt_rev_data,0,sizeof(Mqtt_rev_data));
				Mqtt_rev_length =0;
				printf("Rev %d : %s \n\r",Mqtt_rev_s_length,Mqtt_rev_data_s);
				//{"method":"thing.service.property.set","id":"1345457846","params":{"Current_Limt":32},"version":"1.0.0"} 
				if(strstr((char*)Mqtt_rev_data_s,Current_set1)!=NULL) 
				{
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='t')&&(Mqtt_rev_data_s[i+3]=='1'))
										{
											if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+8]=='}'))
											{
												for(j=0;j<2;j++)
												{
													temp_value[j] = Mqtt_rev_data_s[i+6+j];
												}
												App_Pram_data->Current_set_1 = atof(temp_value);
												
												printf("Current_set_1: %.2f",App_Pram_data->Current_set_1 );
											}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
														for(j=0;j<3;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_1 = atof(temp_value);
												
													printf("Current_set_1: %.2f",App_Pram_data->Current_set_1);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+10]=='}'))
											{
														for(j=0;j<4;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_1 = atof(temp_value);
												
													printf("Current_set_1: %.2f",App_Pram_data->Current_set_1);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+11]=='}'))
											{
														for(j=0;j<5;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_1 = atof(temp_value);
												
													printf("Current_set_1: %.2f",App_Pram_data->Current_set_1);
													savenv();
												}
										}
								}
							
				}
				if(strstr((char*)Mqtt_rev_data_s,Current_set2)!=NULL) 
				{
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='t')&&(Mqtt_rev_data_s[i+3]=='2'))
										{
											if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+8]=='}'))
											{
												for(j=0;j<2;j++)
												{
													temp_value[j] = Mqtt_rev_data_s[i+6+j];
												}
												App_Pram_data->Current_set_2 = atof(temp_value);
												
												printf("Current_set_2: %.2f",App_Pram_data->Current_set_2 );
											}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
														for(j=0;j<3;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_2 = atof(temp_value);
												
													printf("Current_set_2: %.2f",App_Pram_data->Current_set_2);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+10]=='}'))
											{
														for(j=0;j<4;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_2 = atof(temp_value);
												
													printf("Current_set_2: %.2f",App_Pram_data->Current_set_2);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+11]=='}'))
											{
														for(j=0;j<5;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_2 = atof(temp_value);
												
													printf("Current_set_2: %.2f",App_Pram_data->Current_set_2);
													savenv();
												}
										}
								}
							
				}
				if(strstr((char*)Mqtt_rev_data_s,Current_set3)!=NULL) 
				{
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='t')&&(Mqtt_rev_data_s[i+3]=='3'))
										{
											if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+8]=='}'))
											{
												for(j=0;j<2;j++)
												{
													temp_value[j] = Mqtt_rev_data_s[i+6+j];
												}
												App_Pram_data->Current_set_3 = atof(temp_value);
												
												printf("Current_set_3: %.2f",App_Pram_data->Current_set_3 );
											}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
														for(j=0;j<3;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_3 = atof(temp_value);
												
													printf("Current_set_3: %.2f",App_Pram_data->Current_set_3);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+10]=='}'))
											{
														for(j=0;j<4;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_3 = atof(temp_value);
												
													printf("Current_set_3: %.2f",App_Pram_data->Current_set_3);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+11]=='}'))
											{
														for(j=0;j<5;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_3 = atof(temp_value);
												
													printf("Current_set_3: %.2f",App_Pram_data->Current_set_3);
													savenv();
												}
										}
								}
							
				}
				if(strstr((char*)Mqtt_rev_data_s,Current_set4)!=NULL) 
				{
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='t')&&(Mqtt_rev_data_s[i+3]=='4'))
										{
											if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+8]=='}'))
											{
												for(j=0;j<2;j++)
												{
													temp_value[j] = Mqtt_rev_data_s[i+6+j];
												}
												App_Pram_data->Current_set_4 = atof(temp_value);
												
												printf("Current_set_4: %.2f",App_Pram_data->Current_set_4 );
											}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
														for(j=0;j<3;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_4 = atof(temp_value);
												
													printf("Current_set_4: %.2f",App_Pram_data->Current_set_4);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+10]=='}'))
											{
														for(j=0;j<4;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_4 = atof(temp_value);
												
													printf("Current_set_4: %.2f",App_Pram_data->Current_set_4);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+11]=='}'))
											{
														for(j=0;j<5;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Current_set_4 = atof(temp_value);
												
													printf("Current_set_4: %.2f",App_Pram_data->Current_set_4);
													savenv();
												}
										}
								}	
				}
				
				if(strstr((char*)Mqtt_rev_data_s,Relax_Set_Value1)!=NULL) 
				{//Value1
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='l')&&(Mqtt_rev_data_s[i+1]=='u')&&(Mqtt_rev_data_s[i+2]=='e')&&(Mqtt_rev_data_s[i+3]=='1'))
										{
											if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+8]=='}'))
											{
												for(j=0;j<2;j++)
												{
													temp_value[j] = Mqtt_rev_data_s[i+6+j];
												}
												App_Pram_data->Relax1_Set_Value = atof(temp_value);
												
												printf("Relax1_Set_Value: %.2f",App_Pram_data->Relax1_Set_Value );
											}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
														for(j=0;j<3;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Relax1_Set_Value = atof(temp_value);
												
													printf("Relax1_Set_Value: %.2f",App_Pram_data->Relax1_Set_Value);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+10]=='}'))
											{
														for(j=0;j<4;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Relax1_Set_Value = atof(temp_value);
												
													printf("Relax1_Set_Value: %.2f",App_Pram_data->Relax1_Set_Value);
												}
											else if((Mqtt_rev_data_s[i+5]==':')&&(Mqtt_rev_data_s[i+11]=='}'))
											{
														for(j=0;j<5;j++)
														{
															temp_value[j] = Mqtt_rev_data_s[i+6+j];
													
														}
													App_Pram_data->Relax1_Set_Value = atof(temp_value);
												
													printf("Relax1_Set_Value: %.2f",App_Pram_data->Relax1_Set_Value);
													savenv();
												}
										}
								}	
				}	
				else if(strstr((char*)Mqtt_rev_data_s,NTP_Start)!=NULL) 
				{
							for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='N')&&(Mqtt_rev_data_s[i+1]=='T')&&(Mqtt_rev_data_s[i+2]=='P')&&(Mqtt_rev_data_s[i+3]=='_'))
										{
											
											if((Mqtt_rev_data_s[i+10]==':')&&(Mqtt_rev_data_s[i+12]=='}'))
											{
													if(Mqtt_rev_data_s[i+11]=='1')
													{
														App_Pram_data->NTPStart = 1;
													}
													else
													{
														App_Pram_data->NTPStart = 0;
													}
													
												
												printf("NTP_Start: %d\r\n",App_Pram_data->NTPStart );
											}
											
										}
								}
				}
				else if(strstr((char*)Mqtt_rev_data_s,reflash_time)!=NULL) 
				{
						//{"method":"thing.service.property.set","id":"1579352411","params":{"reflash_time":4},"version":"1.0.0"} 
						sscanf( (char*)Mqtt_rev_data_s, "{\"method\":\"thing.service.property.set\",\"id\":\"%d\",\"params\":{\"reflash_time\":%d},\"version\":\"1.0.0\"}",&rev_id,&rev_int_temp);
						App_Pram_data->timestamp = rev_int_temp;
						rev_int_temp =0;
						savenv();
						printf("timestamp: %d\r\n",App_Pram_data->timestamp );
				}
				else if(strstr((char*)Mqtt_rev_data_s,Channel1_enable)!=NULL) 
				{
					//{"method":"thing.service.property.set","id":"1579331601","params":{"reg1_enable":1},"version":"1.0.0"} 
								for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='e')&&(Mqtt_rev_data_s[i+1]=='n')&&(Mqtt_rev_data_s[i+2]=='a')&&(Mqtt_rev_data_s[i+3]=='b'))
										{
											
											if((Mqtt_rev_data_s[i+7]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
													if(Mqtt_rev_data_s[i+8]=='1')
													{
														App_Pram_data->channel1_en = 1;
													}
													else
													{
														App_Pram_data->channel1_en = 0;
													}
													
												savenv();
												printf("channel1_en: %d\r\n",App_Pram_data->channel1_en );
											}
											
										}
								}
				}
				else if(strstr((char*)Mqtt_rev_data_s,Channel2_enable)!=NULL) 
				{
					//{"method":"thing.service.property.set","id":"1579331601","params":{"reg2_enable":1},"version":"1.0.0"} 
								for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='e')&&(Mqtt_rev_data_s[i+1]=='n')&&(Mqtt_rev_data_s[i+2]=='a')&&(Mqtt_rev_data_s[i+3]=='b'))
										{
											
											if((Mqtt_rev_data_s[i+7]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
													if(Mqtt_rev_data_s[i+8]=='1')
													{
														App_Pram_data->channel2_en = 1;
													}
													else
													{
														App_Pram_data->channel2_en = 0;
													}
													
												savenv();
												printf("channel2_en: %d\r\n",App_Pram_data->channel2_en );
											}
											
										}
								}
				
				}
				else if(strstr((char*)Mqtt_rev_data_s,Channel3_enable)!=NULL) 
				{
					//{"method":"thing.service.property.set","id":"1579331601","params":{"reg3_enable":1},"version":"1.0.0"} 
								for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='e')&&(Mqtt_rev_data_s[i+1]=='n')&&(Mqtt_rev_data_s[i+2]=='a')&&(Mqtt_rev_data_s[i+3]=='b'))
										{
											
											if((Mqtt_rev_data_s[i+7]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
													if(Mqtt_rev_data_s[i+8]=='1')
													{
														App_Pram_data->channel3_en = 1;
													}
													else
													{
														App_Pram_data->channel3_en = 0;
													}
													
												savenv();
												printf("channel3_en: %d\r\n",App_Pram_data->channel3_en);
											}
											
										}
								}
				
				}
				else if(strstr((char*)Mqtt_rev_data_s,Channel4_enable)!=NULL) 
				{
					//{"method":"thing.service.property.set","id":"1579331601","params":{"reg4_enable":1},"version":"1.0.0"} 
								for(i=0;i<Mqtt_rev_s_length;i++)
								{
										if((Mqtt_rev_data_s[i]=='e')&&(Mqtt_rev_data_s[i+1]=='n')&&(Mqtt_rev_data_s[i+2]=='a')&&(Mqtt_rev_data_s[i+3]=='b'))
										{
											
											if((Mqtt_rev_data_s[i+7]==':')&&(Mqtt_rev_data_s[i+9]=='}'))
											{
													if(Mqtt_rev_data_s[i+8]=='1')
													{
														App_Pram_data->channel4_en = 1;
													}
													else
													{
														App_Pram_data->channel4_en = 0;
													}
													
												savenv();
												printf("channel4_en: %d\r\n",App_Pram_data->channel4_en );
											}
											
										}
								}
				
				}
			}
			else if(strstr((char*)data,"/ext/ntp/")!=NULL) 
			{///+CMQPUB: 0,"/ext/ntp/a1n7HqCJq4Y/3325262728EB/response",0,0,0,168,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
				
			for(i=0;i<length;i++)
				{
						if((*(data+i)=='"')&&(*(data+i+1)==','))
						{
								if((*(data+i+10)==',')&&(*(data+i+11)=='"'))
								{
										Mqtt_rev_length = (*(data+i+8)-0x30)*10+(*(data+i+9)-0x30);
									
										if(*(data+i+12+Mqtt_rev_length)=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+12+j);
													}
												
										}
										
									
								}
								else if((*(data+i+11)==',')&&(*(data+i+12)=='"'))
								{
									Mqtt_rev_length =(*(data+i+8)-0x30)*100 + (*(data+i+9)-0x30)*10+(*(data+i+10)-0x30);
									
									if(*(data+i+13+Mqtt_rev_length)=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+13+j);
													}
												
										}
								}
						}
				}
				memset(Mqtt_rev_data_s,0,sizeof(Mqtt_rev_data_s));
				Mqtt_rev_s_length =HextoString(Mqtt_rev_data,Mqtt_rev_length,Mqtt_rev_data_s);
				memset(Mqtt_rev_data,0,sizeof(Mqtt_rev_data));
				Mqtt_rev_length =0;
				printf("NTP Rev %d : %s \n\r",Mqtt_rev_s_length,Mqtt_rev_data_s);
				//{"deviceSendTime":1581104096,"serverSendTime":1578389182858,"serverRecvTime":1578389182858}
				server_send_time =0;
				server_rev_time =0;
				Sys_times =0;
				memset(temp_value,0,sizeof(temp_value));
				for(i=0;i<Mqtt_rev_s_length;i++)
				{
					if((Mqtt_rev_data_s[i]=='d')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='v')&&(Mqtt_rev_data_s[i+3]=='i'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
									if((Mqtt_rev_data_s[j] == ',' )&&(Mqtt_rev_data_s[j+1] == '"' )) 
										{
												for(n=0;n<(j-(i+15)-1);n++)
												{
													temp_value[n] = Mqtt_rev_data_s[i+16+n];
												}
												Sys_times = atol(temp_value);
												memset(temp_value,0,sizeof(temp_value));
									
										}
									}
								
						}

				if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='r')&&(Mqtt_rev_data_s[i+3]=='v')&&(Mqtt_rev_data_s[i+4]=='e')&&(Mqtt_rev_data_s[i+5]=='r')&&(Mqtt_rev_data_s[i+6]=='S')&&(Mqtt_rev_data_s[i+7]=='e'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
									if((Mqtt_rev_data_s[j] == ',' )&&(Mqtt_rev_data_s[j+1] == '"' )) 
										{
												for(n=0;n<(j-(i+15)-4);n++)
												{
													temp_value[n] = Mqtt_rev_data_s[i+16+n];
												}
												server_send_time = atol(temp_value);
												memset(temp_value,0,sizeof(temp_value));
								}
							}
						}

				if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='r')&&(Mqtt_rev_data_s[i+3]=='v')&&(Mqtt_rev_data_s[i+4]=='e')&&(Mqtt_rev_data_s[i+5]=='r')&&(Mqtt_rev_data_s[i+6]=='R')&&(Mqtt_rev_data_s[i+7]=='e'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
									if(Mqtt_rev_data_s[j] == '}' ) 
										{
												for(n=0;n<(j-(i+15)-4);n++)
												{
													temp_value[n] = Mqtt_rev_data_s[i+16+n];
												}
												server_rev_time = atol(temp_value);
												memset(temp_value,0,sizeof(temp_value));
											}
										}
						}
					  
					}
					printf("deviceSendTime:%d,serverSendTime:%d,serverRecvTime:%d \r\n",Sys_times,server_send_time,server_rev_time);
			
				local_time = *localtime(&Sys_times);
				printf("Sys_times:%s \r\n",asctime(&local_time));
				local_time = *localtime(&server_send_time);
				local_time.tm_hour = local_time.tm_hour+8;
				printf("server_send_time:%s \r\n",asctime(&local_time));
				local_time = *localtime(&server_rev_time);
				local_time.tm_hour = local_time.tm_hour+8;
				printf("server_rev_time:%s \r\n",asctime(&local_time));
					if(server_send_time)
					{
						Sys_times = server_send_time;
						local_time = *localtime(&server_send_time);
						Times.Hours =local_time.tm_hour+8;
						Times.Minutes =local_time.tm_min;
						Times.Seconds =local_time.tm_mday;
						Dates.Year = local_time.tm_year+1900-2000;
						Dates.Month = local_time.tm_mon+1;
						Dates.Date = local_time.tm_mday;
						Dates.WeekDay =local_time.tm_wday;
					
						HAL_RTC_SetTime(&hrtc, &Times, RTC_FORMAT_BIN);
						HAL_RTC_SetDate(&hrtc, &Dates, RTC_FORMAT_BIN);
						memset(&Times,0,sizeof(RTC_TimeTypeDef));
						memset(&Dates,0,sizeof(RTC_DateTypeDef));
						HAL_RTC_GetTime(&hrtc, &Times, RTC_FORMAT_BIN);
						HAL_RTC_GetDate(&hrtc, &Dates, RTC_FORMAT_BIN);
						printf("Today is %d year, %d mouth, %d date\n\r",Dates.Year,Dates.Month,Dates.Date);
						printf("now is %d : %d : %d : %d : %d \n\r",Times.Hours,Times.Minutes,Times.Seconds,Times.SubSeconds,Times.SecondFraction);
						CMDR_OK=1;
						Delay_break =1;
					}
			}
			else if(strstr((char*)data,"/ota/device/upgrade/")!=NULL) 
			{
				
				/*+CMQPUB: 0,"/ota/device/upgrade/a1n7HqCJq4Y/3325262728EB",0,0,0,818,"7B22636F6465223A2231303030222C2264617461223A7B2273697A65223A32333737362C227369676E223A223564366564393439623933653261666330393561666534663564333962663139222C2276657273696F6E223A22322E3030222C2275726C223A2268747470733A2F2F6F74612E696F742D7468696E672E636E2D7368616E676861692E616C6979756E63732E636F6D2F6F74612F34323930313239373663663436636165333562353338353866303437336534372F636B347778386B6673303030303362376770686F31647439762E62696E3F457870697265733D31353738303637353738264F53534163636573734B657949643D63533875525279353452737A59576E61265369676E61747572653D335279795350517A69664E6576414B703171335437587453364B59253344222C227369676E4D6574686F64223A224D6435222C226D6435223A223564366564393439623933653261666330393561666534663564333962663139227D2C226964223A313537373938313137383032362C226D657373616765223A2273756363657373227D"*/
	
				for(i=0;i<length;i++)
				{
						if((*(data+i)=='"')&&(*(data+i+1)==','))
						{
								if((*(data+i+10)==',')&&(*(data+i+11)=='"'))
								{
										Mqtt_rev_length = (*(data+i+8)-0x30)*10+(*(data+i+9)-0x30);
									
										if(*(data+i+12+Mqtt_rev_length)=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+12+j);
													}
												
										}
										
									
								}
								else if((*(data+i+11)==',')&&(*(data+i+12)=='"'))
								{
									Mqtt_rev_length =(*(data+i+8)-0x30)*100 + (*(data+i+9)-0x30)*10+(*(data+i+10)-0x30);
									
									if(*(data+i+13+Mqtt_rev_length)=='"')
										{
												for(j=0;j<Mqtt_rev_length;j++)
													{
														Mqtt_rev_data[j] =*(data+i+13+j);
													}
												
										}
								}
						}
				}
					//		sscanf( (char*)data+i, "+CMQPUB: 0,%s,0,0,0,%d,%s\r\n",Topic_temp_data,&Mqtt_rev_length,Mqtt_rev_data);
				memset(Mqtt_rev_data_s,0,sizeof(Mqtt_rev_data_s));
				Mqtt_rev_s_length =HextoString(Mqtt_rev_data,Mqtt_rev_length,Mqtt_rev_data_s);
				memset(Mqtt_rev_data,0,sizeof(Mqtt_rev_data));
				Mqtt_rev_length =0;
				printf("Rev %d : %s \n\r",Mqtt_rev_s_length,Mqtt_rev_data_s);
				/*
Rev 410 : {"code":"1000","data":{"size":23776,"sign":"5d6ed949b93e2afc095afe4f5d39bf19","version":"1.2","url":"https://ota.iot-thing.cn-shanghai.aliyuncs.com/ota/429012976cf46cae35b53858f0473e47/ck4xut5cr00023b7ftkz396vn.bin?Expires=1578123968&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=b4qA0WNXF8aIa4KVFJqVw%2FHFvqA%3D","signMethod":"Md5","md5":"5d6ed949b93e2afc095afe4f5d39bf19"},"id":1578037568875,"message":"success"} 
				
				*/
				for(i=0;i<Mqtt_rev_s_length;i++)
				{
					if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='i')&&(Mqtt_rev_data_s[i+2]=='z')&&(Mqtt_rev_data_s[i+3]=='e'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
										if(Mqtt_rev_data_s[j] == ',' ) 
										{
												for(n=0;n<(j-(i+5)-1);n++)
												{
														temp_value[n] = Mqtt_rev_data_s[i+6+n];
												}
												App_Pram_data ->Firmware_size =atoi(temp_value);
												memset(temp_value,0,sizeof(temp_value));
												printf("Firmware_size: %d\r\n",App_Pram_data ->Firmware_size);
												App_Pram_data->Write_Flag =0;
												savenv();

												break;
										}
								
								}
					}
//Rev 410 : {"code":"1000","data":{"size":23776,"sign":"5d6ed949b93e2afc095afe4f5d39bf19","version":"1.2","url":"https://ota.iot-thing.cn-shanghai.aliyuncs.com/ota/429012976cf46cae35b53858f0473e47/ck4xut5cr00023b7ftkz396vn.bin?Expires=1578123968&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=b4qA0WNXF8aIa4KVFJqVw%2FHFvqA%3D","signMethod":"Md5","md5":"5d6ed949b93e2afc095afe4f5d39bf19"},"id":1578037568875,"message":"success"} 
			//9 13
					if((Mqtt_rev_data_s[i]=='s')&&(Mqtt_rev_data_s[i+1]=='i')&&(Mqtt_rev_data_s[i+2]=='g')&&(Mqtt_rev_data_s[i+3]=='n')&&(Mqtt_rev_data_s[i+4]=='"'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
										if((Mqtt_rev_data_s[j] == '"' )&&(Mqtt_rev_data_s[j+1] == ',' )) 
										{
												for(n=0;n<(j-(i+6)-1);n++)
												{
														App_Pram_data ->Firmware_sign[n] =Mqtt_rev_data_s[i+7+n];
												}
												
												printf("Firmware_sign: %s\r\n",App_Pram_data ->Firmware_sign);
												savenv();
												break;
										}
								
								}
					}
//Rev 410 : {"code":"1000","data":{"size":23776,"sign":"5d6ed949b93e2afc095afe4f5d39bf19","version":"1.2","url":"https://ota.iot-thing.cn-shanghai.aliyuncs.com/ota/429012976cf46cae35b53858f0473e47/ck4xut5cr00023b7ftkz396vn.bin?Expires=1578123968&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=b4qA0WNXF8aIa4KVFJqVw%2FHFvqA%3D","signMethod":"Md5","md5":"5d6ed949b93e2afc095afe4f5d39bf19"},"id":1578037568875,"message":"success"} 
					if((Mqtt_rev_data_s[i]=='v')&&(Mqtt_rev_data_s[i+1]=='e')&&(Mqtt_rev_data_s[i+2]=='r')&&(Mqtt_rev_data_s[i+3]=='s'))
					{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
										if((Mqtt_rev_data_s[j] == '"' )&&(Mqtt_rev_data_s[j+1] == ',' )) 
										{
												memset(temp_value,0,sizeof(temp_value));
												for(n=0;n<(j-(i+9)-1);n++)
												{
													temp_value[n] = Mqtt_rev_data_s[i+10+n];
												}
												App_Pram_data->Firmware_Version_new = (atof(temp_value)*100)/100;
												
												memset(temp_value,0,sizeof(temp_value));
												printf("Firmware_Version_new: %.1f\r\n",App_Pram_data ->Firmware_Version_new);
												if(App_Pram_data->Firmware_Version_new>App_Pram_data->Firmware_Version)
												{
														New_version_ready =1;
												}
												savenv();
												break;
										}
								
								}
					}
					
//Rev 410 : {"code":"1000","data":{"size":23776,"sign":"5d6ed949b93e2afc095afe4f5d39bf19","version":"1.2","url":"https://ota.iot-thing.cn-shanghai.aliyuncs.com/ota/429012976cf46cae35b53858f0473e47/ck4xut5cr00023b7ftkz396vn.bin?
					//Expires=1578123968&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=b4qA0WNXF8aIa4KVFJqVw%2FHFvqA%3D","signMethod":"Md5","md5":"5d6ed949b93e2afc095afe4f5d39bf19"},"id":1578037568875,"message":"success"} 
					if(New_version_ready)
					{
						if((Mqtt_rev_data_s[i]=='u')&&(Mqtt_rev_data_s[i+1]=='r')&&(Mqtt_rev_data_s[i+2]=='l')&&(Mqtt_rev_data_s[i+3]=='"')&&(Mqtt_rev_data_s[i+4]==':'))
						{
							for(j=i;j<Mqtt_rev_s_length; j++)
								{
										if((Mqtt_rev_data_s[j] == '"' )&&(Mqtt_rev_data_s[j+1] == ',' )) 
										{
											  memset(App_Pram_data->Up_Server_addr,0,1024);
												for(n=0;n<(j-(i+5)-1);n++)
												{
												App_Pram_data->Up_Server_addr[n] = Mqtt_rev_data_s[i+6+n];
													
												}
												printf("Url: %s\r\n end",App_Pram_data ->Up_Server_addr);
												savenv();
												break;
										}
									}
							}
						}

				}
			}
		}
		if(strstr((char*)data,"+CMQDISCON:")!=NULL)
		{
			printf("MQTT Client closed! \n");
		//	SIM_Init_OK=0;
			MQTT_Client_OK =0;
		}
}

void Sleep(uint32_t Tmsec)
{
	uint32_t i,j,m;
	
	for(i=0;i<Tmsec;i++)
	{
			feed_dog();
			for(j=0;j<1680;j++)
 				for(m=0;m<1024;m++);
	}
}
uint8_t Sleep_break(uint32_t Tmsec)
{
	uint32_t i,j,m;
	
	for(i=0;i<Tmsec;i++)
	{
		//	feed_dog();
			  for(j=0;j<1000;j++)
 				for(m=0;m<1680;m++)
					{
						if(Delay_break)
						{
							return 0;
						}
					}
	}
	return 0;
}
uint8_t Sleep_break_net(uint32_t Tmsec)
{
	uint32_t i,j,m;
	
	for(i=0;i<Tmsec;i++)
	{
		//	feed_dog();
			for(j=0;j<1680;j++)
 				for(m=0;m<1024;m++)
					{
						if(Delay_break_net)
						{
							return 0;
						}
					}
	}
	return 0;
}

 void Pow_up_sim()
 {
 	HAL_GPIO_WritePin(STM_PWRKEY_GPIO_Port,STM_PWRKEY_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(STM_PWRKEY_GPIO_Port,STM_PWRKEY_Pin, GPIO_PIN_RESET);
  }
 
void REST_SIM()
  {
      HAL_GPIO_WritePin(STM_RESET_GPIO_Port,STM_RESET_Pin ,GPIO_PIN_SET);
      HAL_Delay(200);
      HAL_GPIO_WritePin(STM_RESET_GPIO_Port,STM_RESET_Pin ,GPIO_PIN_RESET);
  }


 uint8_t init_SIM(void)
 {
	 char CMD[256];
	 IOT_Step=0;
	 SIM_Init_Flag =1;
	 uint8_t error_counter =0;
	 feed_dog();
	 memset(CMD,0,256);
	 ATE_STATUS =0;
	 feed_dog();
	 if(ATE_STATUS ==0)
		 {
			 IOT_Step =0;
				/*Close ATE*/
				Delay_break =0;
				while(IOT_Step ==0)
				{
					feed_dog();
					if(IOT_GSM)  //"SIM7020C"   
					{
							sprintf(CMD,"%s\r\n",ATE);
							HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					}
					else
					{
							sprintf(CMD,"%s\r\n",ATE);
							HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					}
					error_counter++;
					if(error_counter>5)
					{
						return ERROR;
					}
					Sleep_break(800);
				}
			}
		 
			error_counter =0;
			
			 memset(CMD,0,256);
			AT_ready =0;
			if(AT_ready==0)
			{
				IOT_Step =0;
				Delay_break =0;
				while(IOT_Step==0)
				{
				  /*Send "AT" Test Module is Working*/
					if(IOT_GSM)  //"SIM7020C"   
					{
							sprintf(CMD,"%s\r\n",AT);
							HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					}	
					else
					{
							sprintf(CMD,"%s\r\n",AT);
						 	HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
						IOT_Step =1;
					}
					error_counter++;
					if(error_counter>5)
					{
						return ERROR;
					}
					Sleep_break(300);
					}
			}
			feed_dog();
		error_counter =0;
			
			 memset(CMD,0,256);
			Sim_ready =0;
			if(Sim_ready==0)
			{
				IOT_Step =0;
				Delay_break =0;
				while(IOT_Step==0)
				{
					feed_dog();
					/*Send "AT+CPIN?" Test Module is Working*/
					if(IOT_GSM)  //"SIM7020C"
					{
						sprintf(CMD,"%s\r\n",CPIN);
						HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
						//memcpy(CMD,"AT+CPIN?\n\r",sizeof("AT+CPIN?\n\r"));
						//HAL_UART_Transmit(&huart2, (uint8_t *)CMD, sizeof("AT+CPIN?\n\r"), 0xFFFF);
					}
					else
					{
					//	sprintf(CMD,"%s\n\r",CPIN);
					//	HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
						Sim_ready =1;
						IOT_Step =1;
						Delay_break=1;
					//	memcpy(CMD,"AT+CPIN?\n\r",sizeof("AT+CPIN?\n\r"));
					//	HAL_UART_Transmit(&huart2, (uint8_t *)CMD, sizeof("AT+CPIN?\n\r"), 0xFFFF);
					}
					error_counter++;
					if(error_counter>5)
					{
						return ERROR;
					}
					Sleep_break(300);	
					}
			}
			feed_dog();
			SIM_Init_Flag=0;
		memset(CMD,0,256);
			CSQ_Status =0;
		if(CSQ_Status==0)
		{
				/*Get CSQ data */
				IOT_Step =0;
				Delay_break =0;
				if(IOT_GSM)  //"SIM7020C"
				{
						sprintf(CMD,"%s\r\n",CSQ);
						HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					
				//		memcpy(CMD,"AT+CSQ\n\r",sizeof("AT+CSQ?\n\r"));
				//		HAL_UART_Transmit(&huart2, (uint8_t *)CMD, sizeof("AT+CSQ?\n\r"), 0xFFFF);
					}
				else
					{
						sprintf(CMD,"%s\r\n",CSQ);
						HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
						//memcpy(CMD,"AT+CSQ\n\r",sizeof("AT+CSQ?\n\r"));
					//	HAL_UART_Transmit(&huart2, (uint8_t *)CMD, sizeof("AT+CSQ?\n\r"), 0xFFFF);
					}
				Sleep_break(300);	
			}
		feed_dog();
		 memset(CMD,0,256);
			Cmnbiot_ready =0;
	  if(Cmnbiot_ready==0)
		{		/*Get Module IP*/
				Delay_break =0;
				if(IOT_GSM)  //"SIM7020C"
				{
						sprintf(CMD,"%s\r\n",CGCONTRDP);
					
						HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					}
				else
				{
						sprintf(CMD,"%s\r\n",CGCONTRDP);
						HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
					}
			Sleep_break(200);	
		}
		
		memset(CMD,0,256);
		ServerIP_ready =0;
		if(ServerIP_ready==0)
		{   /*Get Module IP*/
				Delay_break =0;
				if(IOT_GSM)  //"SIM7020C"
				{
						sprintf(CMD,"%s=%s\r\n",DNS,"ota.iot-thing.cn-shanghai.aliyuncs.com");
						HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen(CMD), 0xFFFF);
				}
				else
				{
						ServerIP_ready =1;
						Delay_break =1;
					}
			Sleep_break(400);	
		}
		if(IOT_GSM) 
				printf("server IP = %s\r\n",ServerIP);
		feed_dog();
	//	printf("Server_url= %s\r\n",Server_url);
		if(ServerIP_ready)
		{
				SIM_Init_OK=1;
				printf("IOT init OK ! \r\n");
				return SUCCESS;
		}
		else
		{
				return	ERROR ;
		}
		
 }

void StringtoHex(char *datain,uint16_t length,char *dataout,uint16_t length_out)
{
	uint16_t i;
	
		for(i=0;i<length;i++)
	{
			if((datain[i]/0x10)>9)
			{
				dataout[i*2] = 0x41+(datain[i]/0x10 -10);
			}	
			else
			{
					dataout[i*2] = 0x30+datain[i]/0x10;
			}
			if((datain[i]%0x10)>9)
			{
					dataout[i*2+1] = 0x41+(datain[i]%0x10-10);
			}	
			else
			{
					dataout[i*2+1] = 0x30+datain[i]%0x10;
				}
		}
	length_out = length*2;

}


 void SPI_flash_test()
 {
	
	uint32_t i = 0;	
	unsigned char WriteBuffer[200];
	unsigned char ReadBuffer[200];
	 unsigned char read_test;
 	for(i = 0;i < 200;i ++)//初始化待写数据区
	{
		WriteBuffer[i] = i;
	}
	BSP_W25Qx_Erase_APP_Block(Firmware_SPI_flash_addr);
		for(i=0;i<0x254B0;i++)
	{
		BSP_W25Qx_Read(&read_test,Firmware_SPI_flash_addr+i,1);
		if(read_test!=0xff)
		{
				printf(" erase fail!!\n\r");
			  return;
		}
	}
	printf("SPI Flash 正在进行读写测试\n\r");
	BSP_W25Qx_Read(ReadBuffer,Firmware_SPI_flash_addr,200);
	BSP_W25Qx_Write( WriteBuffer, Firmware_SPI_flash_addr, 200);
  BSP_W25Qx_Read(ReadBuffer,Firmware_SPI_flash_addr,200);
 
	for(i=0;i<200;i++)
	{
		if(ReadBuffer[i]!=WriteBuffer[i])
		{
				printf(" SPI Flash 读写测试失败！\n\r");
			  return;
		}
	}
  printf(" SPI Flash  读写测试成功！\n\r");
 }
 
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t ret,pindata;
	uint8_t SIM_CD;
	uint8_t delay_conter=0;
	uint8_t SPI_flash_ID[4];
  int i ;
	uint8_t  j;
	uint32_t pub_id =0;
	uint8_t LED_flag=1;
	uint8_t Flash_read_test;
	uint8_t 	result;
	uint16_t  Input_Result[4];
	float 	humidity;
	float 	temperature;
	
	uint32_t loader_https_start,m;
	uint32_t loader_https_end;
	uint32_t loadering_counter;
	
	float   CurrentTemperature=0;
	char    topic_data[1024];
	char    topic_data_hex[2048];
	uint16_t data_hex_length;
	uint16_t   Rreg[2];  //
	uint32_t   Rreg_temp;
	uint32_t   Rreg_temp_rev;	
	uint32_t   float_temp;
//	unsigned char secret[] = "sDRUvop8Njyuycbd";
//  unsigned char buffer[] = "clientIdmyclientdeviceName3325262728EBproductKeya1n7HqCJq4Ytimestamp789";// "clientIdmyclientdeviceName3325262728EBproductKeya1n7HqCJq4Ytimestamp789";
//  unsigned char digest[20];
//  

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

	MX_GPIO_Init();

  /* Initialize all configured peripherals */
  
//  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();

  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
	ModbusMaster_begin();
	HAL_UART_Transmit(&huart6, "Hello World!\r\n", 14, 0xFFFF);
  MX_RTC_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
//	while(1);
	feed_dog();
  printf("App start ......\n");
	BSP_W25Qx_Init();
  BSP_W25Qx_Read_ID(SPI_flash_ID);
	feed_dog();
	printf("Flash ID 0x%02x%02x \r\n",SPI_flash_ID[0],SPI_flash_ID[1]);
	//SPI_flash_test();
	
//	while(1);
	Boot_pram_data =( Boot_pram *)0x10008000;
	FLASH_Read(Boot_pram_addr,(uint8_t*)Boot_pram_data,sizeof(Boot_pram));
	while(Boot_pram_data->Write_Flag!=0x55)
	{
			memset(Boot_pram_data,0,sizeof(Boot_pram));
			Boot_pram_data->Hardware_vesion = Boot_Hversion;
			Boot_pram_data->Firmware_Version = Boot_Fversion;
			Boot_pram_data->App_updata_Ready =0;
			Boot_pram_data->Write_Flag =0x55;
		  Flash_write(Boot_pram_addr ,(uint8_t *)Boot_pram_data, sizeof(Boot_pram),10,1);
			Boot_pram_data->Write_Flag =0;
		  FLASH_Read(Boot_pram_addr,(uint8_t *)Boot_pram_data,sizeof(Boot_pram));
	}
	App_Pram_data =( App_Pram *)(0x10008000+sizeof(Boot_pram));
	FLASH_Read(App_pram_addr,(uint8_t*)App_Pram_data,sizeof(App_Pram));
	while(App_Pram_data->Write_Flag!=0x55)
	{
			memset(App_Pram_data,0,sizeof(App_Pram));
			App_Pram_data->Hardware_vesion = Hversion;
			App_Pram_data->Firmware_Version = Fversion;
			sprintf(App_Pram_data->ProductKey,"%s",ProductKey_1);
			sprintf(App_Pram_data->ProductSecret,"%s",DeviceSecret_1);
			sprintf(App_Pram_data->Product_Id,"%s",Product_Id_1);
			sprintf(App_Pram_data->Device_Name,"%s",DeviceName_1);
			sprintf(App_Pram_data->clientId,"%s",MQTT_client_id_1);
			sprintf(App_Pram_data->Firmware_sign,"%s","61a62c35363c521541bf17a9a9e455be");			
			sprintf(App_Pram_data->Up_Server_addr,"%s",Https_SERVER_NAME);
			App_Pram_data->Firmware_size=0;
			App_Pram_data->Firmware_Version_new =0;
			App_Pram_data->timestamp =timestamp_1;  
			App_Pram_data->Modbus_burrate =0;
		  App_Pram_data->Modbus_Check =0;
		  App_Pram_data->Modbus_databit =0;
		  App_Pram_data->Modbus_Stopbit =0;
	
			App_Pram_data->Write_Flag =0x55;
		  Flash_write(App_pram_addr ,(uint8_t *)App_Pram_data, sizeof(App_Pram),11,1);
			App_Pram_data->Write_Flag =0;
		  FLASH_Read(App_pram_addr,(uint8_t *)App_Pram_data,sizeof(App_Pram));
	}
	MX_USART6_UART_Init();
	feed_dog();
//	BSP_W25Qx_Erase_Block(0x30000);
	if(Fversion >= App_Pram_data->Firmware_Version_new-0.1)
	{
		Boot_pram_data->App_updata_Ready =0;
		savebootnv();
	}
    /* Start Times */
  HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim3);
   /* Start huart1 IT rev */
  if(HAL_UART_Receive_IT(&huart1,&huart1_buf,1)==HAL_OK)
  {
    huart1_rev_flag =1;
  }
  else
  {
    huart1_rev_flag =0;
  }
  
  if(HAL_UART_Receive_IT(&huart2,&huart2_buf,1)==HAL_OK)
  {
    huart2_rev_flag =1;
  }
  else
  {
    huart2_rev_flag =0;
  }
  
   if(HAL_UART_Receive_IT(&huart6,&huart6_buf,1)==HAL_OK)
  {
    huart6_rev_flag =1;
  }
  else
  {
    huart6_rev_flag =0;
  }
	
	if(IOT_GSM) 
	{
		Wait_times =3;          
	}
	else
	{
		Wait_times =5; 
	}
  
	feed_dog();
	
  SIM_CD = HAL_GPIO_ReadPin(SIM_CD_GPIO_Port,SIM_CD_Pin);
	if(SIM_CD==0)
	{
		printf("SIM Card NO In\r\n");
	}
	else
	{
		printf("SIM Card IS Ready\r\n");
	}
			
	SIM_Init_OK =0;

	while(SIM_Init_OK==0)
	{
			Pow_up_sim();
			feed_dog();
			REST_SIM();

		if(IOT_GSM)
		{
			feed_dog();
			HAL_Delay(1000);
			pindata = HAL_GPIO_ReadPin(STM_STA_GPIO_Port,STM_STA_Pin);
			printf("start delay\r\n");
			feed_dog();
			delay_conter = 0;
		while( pindata == 0)
		{
			pindata = HAL_GPIO_ReadPin(STM_STA_GPIO_Port,STM_STA_Pin);
			printf(".. ");
			HAL_Delay(100);
			delay_conter++;
			if(delay_conter>10)
			{
				break;
			}
			feed_dog();
			}
			printf("END delay\r\n");
		}
		else
		{
				printf("start delay\r\n");
				for(i=0;i<10;i++)
				{
					feed_dog();
					HAL_Delay(1000);
					feed_dog();
				}
				printf("END delay\r\n");
			}
		
		
		//IO 复位
			feed_dog();
			init_SIM();	
	}
	printf("init_SIM OK!\n\r");
 // init_GPS();
	ret = MQTT_Link();
	ret = MQTT_sub_topic();
while(1){
			SIM_CD = HAL_GPIO_ReadPin(SIM_CD_GPIO_Port,SIM_CD_Pin);
			if(SIM_CD==0)
			{
				printf("SIM Card NO In\r\n");
			}
			else
			{
				printf("SIM Card IS Ready\r\n");
			}
	
			feed_dog();
	if(runticks)
	{
   Cheart++;
		if(Cheart>0x800000)
		{
			Cheart =0;
		}
		HAL_GPIO_WritePin(ERROR_GPIO_Port,ERROR_Pin, GPIO_PIN_RESET);		
				feed_dog();
					if(pub_id>0x8000)
					{
							pub_id =0;
					}
		 result = ModbusMaster_readHoldingRegisters(0x01,0x0, 22);
     if (result == 0x00)
		{
			
			feed_dog();
			Rreg[0] = ModbusMaster_getResponseBuffer(0x00);
			Rreg[1] = ModbusMaster_getResponseBuffer(0x01);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Main_value_channel1,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(0x02);
			Rreg[1] = ModbusMaster_getResponseBuffer(0x03);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu1_value_channel1,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(0x04);
			Rreg[1] = ModbusMaster_getResponseBuffer(0x05);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu2_Value_channel1,&Rreg_temp,4);
	
			feed_dog();
			Rreg[0] = ModbusMaster_getResponseBuffer(0x06);
			Rreg[1] = ModbusMaster_getResponseBuffer(0x07);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Main_value_channel2,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(0x08);
			Rreg[1] = ModbusMaster_getResponseBuffer(0x09);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu1_value_channel2,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(10);
			Rreg[1] = ModbusMaster_getResponseBuffer(11);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu2_Value_channel2,&Rreg_temp,4);
			
			feed_dog();
			Rreg[0] = ModbusMaster_getResponseBuffer(12);
			Rreg[1] = ModbusMaster_getResponseBuffer(13);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Main_value_channel3,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(14);
			Rreg[1] = ModbusMaster_getResponseBuffer(15);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu1_value_channel3,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(16);
			Rreg[1] = ModbusMaster_getResponseBuffer(17);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu2_Value_channel3,&Rreg_temp,4);

			feed_dog();
			Rreg[0] = ModbusMaster_getResponseBuffer(18);
			Rreg[1] = ModbusMaster_getResponseBuffer(19);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Main_value_channel4,&Rreg_temp,4);

			Rreg[0] = ModbusMaster_getResponseBuffer(20);
			Rreg[1] = ModbusMaster_getResponseBuffer(21);
			Rreg_temp =(Rreg[1]*0x10000+Rreg[0]);
			memcpy(&App_Pram_data->Fu1_value_channel4,&Rreg_temp,4);
			
			memset(topic_data,0,sizeof(topic_data));
			memset(topic_data_hex,0,sizeof(topic_data_hex));
			sprintf(topic_data,"{\"id\":\"%d\",\"version\":%.1f,params:{Main_value_channel1:%.2f,Fu1_value_channel1:%.2f,Fu2_Value_channel1:%.2f,\
			Main_value_channel2:%.2f,Fu1_value_channel2:%.2f,Fu2_Value_channel2:%.2f\
			,Main_value_channel3:%.2f,Fu1_value_channel3:%.2f,Fu2_Value_channel3:%.2f\
			,Main_value_channel4:%.2f,Fu1_value_channel4:%.2f}}",pub_id++,Mqtt_version,App_Pram_data->Main_value_channel1,App_Pram_data->Fu1_value_channel1,App_Pram_data->Fu2_Value_channel1,
			App_Pram_data->Main_value_channel2,App_Pram_data->Fu1_value_channel2,App_Pram_data->Fu2_Value_channel2,
			App_Pram_data->Main_value_channel3,App_Pram_data->Fu1_value_channel3,App_Pram_data->Fu2_Value_channel3,
			App_Pram_data->Main_value_channel4,App_Pram_data->Fu1_value_channel4
			);
			
			if(IOT_GSM) 
			{
				StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
				MQTT_pub_attrib_topic(topic_data_hex,strlen(topic_data_hex));
				}
			else
			{
				MQTT_pub_attrib_topic(topic_data,strlen(topic_data));
				}
		
  	}	
		else
		{
			App_Pram_data->Main_value_channel1 =22.11;
			App_Pram_data->Fu1_value_channel1 =33.21;
			App_Pram_data->Fu2_Value_channel1 =0.01;
			App_Pram_data->Main_value_channel2 =22.12;
			App_Pram_data->Fu1_value_channel2 =33.22;
			App_Pram_data->Fu2_Value_channel2 =0.02;		
			App_Pram_data->Main_value_channel3 =22.13;
			App_Pram_data->Fu1_value_channel3 =33.23;
			App_Pram_data->Fu2_Value_channel3 =0.03;
			App_Pram_data->Main_value_channel4 =22.14;
			App_Pram_data->Fu1_value_channel4 =33.24;
			
			memset(topic_data,0,sizeof(topic_data));
			memset(topic_data_hex,0,sizeof(topic_data_hex));
			sprintf(topic_data,"{\"id\":\"%d\",\"version\":%.1f,params:{Main_value_channel1:%.2f,Fu1_value_channel1:%.2f,Fu2_Value_channel1:%.2f,\
			Main_value_channel2:%.2f,Fu1_value_channel2:%.2f,Fu2_Value_channel2:%.2f\
			,Main_value_channel3:%.2f,Fu1_value_channel3:%.2f,Fu2_Value_channel3:%.2f\
			,Main_value_channel4:%.2f,Fu1_value_channel4:%.2f}}",pub_id++,Mqtt_version,App_Pram_data->Main_value_channel1,App_Pram_data->Fu1_value_channel1,App_Pram_data->Fu2_Value_channel1,
			App_Pram_data->Main_value_channel2,App_Pram_data->Fu1_value_channel2,App_Pram_data->Fu2_Value_channel2,
			App_Pram_data->Main_value_channel3,App_Pram_data->Fu1_value_channel3,App_Pram_data->Fu2_Value_channel3,
			App_Pram_data->Main_value_channel4,App_Pram_data->Fu1_value_channel4
			);		
			if(IOT_GSM) 
			{
				StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
				MQTT_pub_attrib_topic(topic_data_hex,strlen(topic_data_hex));
				}
			else
			{
				MQTT_pub_attrib_topic(topic_data,strlen(topic_data));
				}
			}
			HAL_GPIO_WritePin(ERROR_GPIO_Port,ERROR_Pin, GPIO_PIN_SET);	
		}
			if(reset_flag==1)
			{
					feed_dog();
					printf("reinit sim7020\r\n");
					SIM_Init_OK =0;
					while(SIM_Init_OK==0)
				{
					Pow_up_sim();
					feed_dog();
					REST_SIM();
					HAL_Delay(1000);
					feed_dog();
					//IO 复位
					init_SIM();	
					feed_dog();
					}
					printf("init_SIM OK!\n\r");
					// init_GPS();
			}
			if(MQTT_Client_OK==0)
			{
					feed_dog();
					ret = MQTT_Link();
					feed_dog();
					ret = MQTT_sub_topic();
					feed_dog();
			}
			
			LED_Numb_Flag = LED_Numb_Flag|0x01; //run led
	
			if(App_Pram_data->Firmware_Version_new>App_Pram_data->Firmware_Version)
					{
						feed_dog();
					
						if((App_Pram_data->downloading ==1)&&(Start_upgr==0))
						{
									Start_upgr =1;
									loadering_counter =0;
									loader_https_start =App_Pram_data->downloadoffset;
									loader_https_end=0;
									App_Write_Offset=App_Pram_data->write_offset;
						}
						if((App_Pram_data->downloading ==0)&&(Start_upgr==0))
						{
								App_Pram_data->downloading=1;
								Start_upgr =1;
								BSP_W25Qx_Erase_APP_Block(Firmware_SPI_flash_addr);
								for(m=0;m<App_Pram_data->Firmware_size;m++)
									{
												BSP_W25Qx_Read( &Flash_read_test,Firmware_SPI_flash_addr+m,1);
												if(Flash_read_test!=0xFF)
												{
													printf("erase error!\r\n");
													App_Pram_data->downloading =0;
													Start_upgr =0;
													break;
												}
									
									}
								loadering_counter =0;
								loader_https_start =0;
								loader_https_end=0;
								
								
								App_Write_Offset=0;
								printf("Erase end start download!\r\n");
						}
					}
			if(Start_upgr)
				{
					feed_dog();
					ret = Https_init();
					feed_dog();
					if(ret)
					{
								feed_dog();
								if((loader_https_start >loader_https_end)||(loader_https_start==loader_https_end))
									{
										if(App_Pram_data->Firmware_size>loader_https_start)
										{
												if((App_Pram_data->Firmware_size - loader_https_start)>511)
												{
														loader_https_end =loader_https_start+511;
													}
												else
												{
														loader_https_end =App_Pram_data->Firmware_size-1;
													}
											}
										}
									App_Pram_data->downloadoffset = loader_https_start;
									App_Pram_data->write_offset = App_Write_Offset;
									feed_dog();
									ret =Https_download(loader_https_start,loader_https_end);
									feed_dog();
									if(ret)
									{
											loadering_counter++;
											loader_https_start = loader_https_end+1;
										}
									if(loader_https_start == App_Pram_data->Firmware_size)
									{
											feed_dog();
											if(md5Check(App_Pram_data->Firmware_sign))
											{
												feed_dog();
												App_Pram_data->downloading=0;
												Start_upgr =0;
												App_Pram_data->Firmware_Version =App_Pram_data->Firmware_Version_new;
												savenv();
												Boot_pram_data->App_updata_Ready =1;
												savebootnv();
												printf("downloader OK! Please reboot device\n\r");
												while(1);//等待重启
											}
											else
											{
												feed_dog();
												App_Pram_data->downloading=0;
												printf("downloader Error! Please reboot device\n\r");
												loadering_counter =0;
												loader_https_start =0;
												loader_https_end=0;
												BSP_W25Qx_Erase_APP_Block(Firmware_SPI_flash_addr);
												App_Write_Offset=0;
												
											}
									}
					}
				else
				{
				
				}
			}
			feed_dog();
		 if(LED_Flash_flag)
		 {
			 feed_dog();
			 if(LED_Numb_Flag&0x01)    //LED runing
			 {
						if(LED_Status&0x01)
						{
							HAL_GPIO_WritePin(RUN_GPIO_Port,RUN_Pin, GPIO_PIN_SET);
							LED_Status =LED_Status & (~(0x01));
						}
						else
						{ 
							HAL_GPIO_WritePin(RUN_GPIO_Port,RUN_Pin, GPIO_PIN_RESET);
							LED_Status =LED_Status | 0x01;
						}
			 }
			 if(LED_Numb_Flag&0x02)     //LED linking
			 {
						if(LED_Status&0x02)
						{
							HAL_GPIO_WritePin(Link_GPIO_Port,Link_Pin, GPIO_PIN_SET);
							LED_Status =LED_Status & (~(0x02));
						}
						else
						{
							HAL_GPIO_WritePin(Link_GPIO_Port,Link_Pin, GPIO_PIN_RESET);
							LED_Status =LED_Status | 0x02;
						}
			 }
			 if(LED_Numb_Flag&0x04)      //LED Error
			 {
						if(LED_Status&0x04)
						{
							HAL_GPIO_WritePin(ERROR_GPIO_Port,ERROR_Pin, GPIO_PIN_SET);
							LED_Status =LED_Status & (~(0x04));
						}
						else
						{
							HAL_GPIO_WritePin(ERROR_GPIO_Port,ERROR_Pin, GPIO_PIN_RESET);
							LED_Status =LED_Status | 0x04;
						}
			 }
			 if(LED_Numb_Flag&0x08)       // LED modbus linking 
			 {
						if(LED_Status&0x08)
						{
		//					HAL_GPIO_WritePin(GPIOD,LED_RF_TX_Pin, GPIO_PIN_RESET);
							LED_Status =LED_Status & (~(0x08));
						}
						else
						{
	//						HAL_GPIO_WritePin(GPIOD,LED_RF_TX_Pin, GPIO_PIN_SET);
							LED_Status =LED_Status | 0x08;
						}
			 }
			 if(LED_Numb_Flag&0x10)       // LED sim OK  
			 {
						if(LED_Status&0x10)
						{
	//						HAL_GPIO_WritePin(GPIOD,SIM_Ready, GPIO_PIN_RESET);
							LED_Status =LED_Status & (~(0x10));
						}
						else
						{
		//					HAL_GPIO_WritePin(GPIOD,SIM_Ready, GPIO_PIN_SET);
							LED_Status =LED_Status | 0x10;
						}
			 }
					LED_Flash_flag =0;
				//	HAL_TIM_Base_Start_IT(&htim3);
			}
			
			if((systicks)||(App_Pram_data->NTPStart))  //系统程序固件 一天一次
			{
				//NTP
				feed_dog();
				memset(topic_data,0,sizeof(topic_data));
				memset(topic_data_hex,0,sizeof(topic_data_hex));
			  memset(&Times,0,sizeof(RTC_TimeTypeDef));
				memset(&Dates,0,sizeof(RTC_DateTypeDef));
				HAL_RTC_GetTime(&hrtc, &Times, RTC_FORMAT_BIN);
			  HAL_RTC_GetDate(&hrtc, &Dates, RTC_FORMAT_BIN);
				local_time.tm_year= Dates.Year+100;
				local_time.tm_mon= Dates.Month-1;
				local_time.tm_mday = Dates.Date;
				local_time.tm_hour  = Times.Hours;
				local_time.tm_min = Times.Minutes;
				local_time.tm_sec = Times.Seconds;
				local_time.tm_isdst =0;
				printf("now time:%s \r\n",asctime(&local_time));
				Sys_times = mktime(&local_time);
				sprintf(topic_data,"{\"deviceSendTime\":%d}",Sys_times);
				printf("NTP topic: %s \r\n",topic_data);
				NTP_return_ERR =1;
					if(IOT_GSM) 
					{
						StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
						
						MQTT_pub_NTP_topic(topic_data_hex,strlen(topic_data_hex));
					}
					else
					{
						MQTT_pub_NTP_topic(topic_data,strlen(topic_data));
					}
					feed_dog();
				NTP_return_ERR =0;
				App_Pram_data->NTPStart =0;
				systicks =0;
			memset(topic_data,0,sizeof(topic_data));
			memset(topic_data_hex,0,sizeof(topic_data_hex));
			sprintf(topic_data,"{\"id\":\"%d\",\"version\":%.1f,params:{NTP_Start:%d}}",pub_id,Mqtt_version,App_Pram_data->NTPStart);
	//		sprintf(topic_data,"{params:{DeviceID:\"%s\",NTP_Start:%d}}",App_Pram_data->Product_Id,App_Pram_data->NTPStart);
			feed_dog();
			if(IOT_GSM) 
			{
			StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
			MQTT_pub_attrib_topic(topic_data_hex,strlen(topic_data_hex));
			}
			else
			{
				MQTT_pub_attrib_topic(topic_data,strlen(topic_data));
			}
				feed_dog();
				HAL_TIM_Base_Start_IT(&htim7);
					
					
			}	
			
			
		if(runticks)
		{
			feed_dog();
			Read_CQS();
			feed_dog();
			memset(topic_data,0,sizeof(topic_data));
			memset(topic_data_hex,0,sizeof(topic_data_hex));
			sprintf(topic_data,"{params:{DeviceID:\"%s\",lqi:%d,zsta:%d,HVersion:%.1f,Heart_counter:%d}}",App_Pram_data->Product_Id,CSQ_Strength,MQTT_Client_OK,App_Pram_data->Hardware_vesion,Cheart);
			feed_dog();
			if(IOT_GSM) 
			{
			StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
			MQTT_pub_attrib_topic(topic_data_hex,strlen(topic_data_hex));
			}
			else
			{
				MQTT_pub_attrib_topic(topic_data,strlen(topic_data));
			}
 
			if(Start_upgr)
			{
				memset(topic_data,0,sizeof(topic_data));
				memset(topic_data_hex,0,sizeof(topic_data_hex));
  			sprintf(topic_data,"{id:%d,params:{step:%d,desc:OK}}", ++pub_id,(loader_https_start*100/App_Pram_data->Firmware_size));
				if(IOT_GSM) 
				{
					StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
					MQTT_pub_OTAProg_topic(topic_data_hex,strlen(topic_data_hex));
				}
				else
					{
						MQTT_pub_OTAProg_topic(topic_data,strlen(topic_data));
				}
			}
			else
			{
			pub_id =pub_id+1;
			if(pub_id>0x8000)
			{
				pub_id =0;
			}
			memset(topic_data,0,sizeof(topic_data));
			memset(topic_data_hex,0,sizeof(topic_data_hex));
			sprintf(topic_data,"{id:%d,params:{version:%.1f}}", pub_id,Fversion);
			feed_dog();
			//AT+QMTPUB=0,1,1,0,"/ota/device/inform/a1XSjsAfwUn/LLxNet/","{id:1001,params:{version:1.25}}"
			if(IOT_GSM) 
			{
			StringtoHex(topic_data,strlen(topic_data),topic_data_hex,data_hex_length);
			MQTT_pub_OTAinfo_topic(topic_data_hex,strlen(topic_data_hex));
			}
			else
			{
				MQTT_pub_OTAinfo_topic(topic_data,strlen(topic_data));
			}		
			}
			feed_dog();
			runticks=0;
		}
		feed_dog();
		HAL_Delay(1000);
		feed_dog();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x18;
  sTime.Minutes = 0x47;
  sTime.Seconds = 0x30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x7;
  sDate.Year = 0x20;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A 
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
   /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 20*1024-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2*1024;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 21*1024-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4*1024;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 21*1024-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 4*1024;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 21*1024-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4*1024;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 21*1024-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 8;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 21*1024-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 4*1024;
	htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 21*1024-1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 4*1024;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 21*1024-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 8*1024;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 21*1024-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 4*1024;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 21*1024-1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 4*1024;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 21*1024-1;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 4*1024;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */

}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 21*1024-1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 4*1024;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 21*1024-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 4*1024;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */
	uint32_t			Modbus_burrate;   //0: 9600 1: 4800 2: 57600 3: 115200
	uint8_t				Modbus_databit;   //0: 8bit 1:9bit
	uint8_t				Modbus_Stopbit;    // 0:1bit 1:2bit
	uint8_t				Modbus_Check;      //0: none 1:odd 2:even
  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
	switch(App_Pram_data->Modbus_burrate)
	{
		case 0:
			huart6.Init.BaudRate = 9600;
			break;
		case 1:
			huart6.Init.BaudRate = 4800;
			break;
		case 2:
			huart6.Init.BaudRate = 57600;
			break;	
		case 3:
			huart6.Init.BaudRate = 115200;
			break;		
		default:
			huart6.Init.BaudRate = 9600;
			break;
	}
	switch(App_Pram_data->Modbus_databit)
	{
		case 0:
			huart6.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		case 1:
			huart6.Init.WordLength = UART_WORDLENGTH_9B;
			break;
		default:
			huart6.Init.WordLength = UART_WORDLENGTH_8B;
			break;
	}
  	switch(App_Pram_data->Modbus_Stopbit)
	{
		case 0:
			huart6.Init.StopBits = UART_STOPBITS_1;
			break;
		case 1:
			huart6.Init.StopBits = UART_STOPBITS_2;
			break;
		default:
			huart6.Init.StopBits = UART_STOPBITS_1;
			break;
	}
	 switch(App_Pram_data->Modbus_Check)
	{
		case 0:
			huart6.Init.Parity = UART_PARITY_NONE;
			break;
		case 1:
			huart6.Init.Parity = UART_PARITY_ODD;
			break;
		case 2:
			huart6.Init.Parity = UART_PARITY_EVEN;
			break;
		default:
			huart6.Init.Parity = UART_PARITY_NONE;
	}
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
	__HAL_UART_ENABLE_IT(&huart6,UART_IT_RXNE);
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

 
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_WDI_GPIO_Port, MCU_WDI_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, STM_RESET_Pin|STM_PWRKEY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ERROR_Pin|RUN_Pin|Link_Pin|SPI_NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : MCU_WDI_Pin */
  GPIO_InitStruct.Pin = MCU_WDI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MCU_WDI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GS_PSM_EINT_Pin STM_STA_Pin */
  GPIO_InitStruct.Pin = GS_PSM_EINT_Pin|STM_STA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SIM_CD_Pin */
  GPIO_InitStruct.Pin = SIM_CD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SIM_CD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STM_RESET_Pin STM_PWRKEY_Pin ERROR_Pin Link_Pin 
                           SPI_NSS_Pin */
  GPIO_InitStruct.Pin = STM_RESET_Pin|STM_PWRKEY_Pin|ERROR_Pin|Link_Pin 
                          |SPI_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RUN_Pin */
  GPIO_InitStruct.Pin = RUN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RUN_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
     if(UartHandle->Instance ==huart1.Instance)   //uart2
      {

			}
     else if(UartHandle->Instance ==huart2.Instance)   //uart2
      {

          SIM_UART_Timeout =0; 
          SIM_DATA_BUF[SIM_Buf_length++] = huart2_buf;
					
					if(SIM_Buf_length==SIM_REV_BUF_MAX)
					{
								printf("GSM_Status_Check SIM_DATA_BUF SIM_Buf_length =%d",SIM_Buf_length);
						
								GSM_Status_Check(SIM_DATA_BUF,SIM_Buf_length);
                memset(SIM_DATA_BUF,0,SIM_REV_BUF_MAX);
                SIM_Buf_length =0;
								
					}
					else
					{
							  HAL_TIM_Base_Start_IT(&htim2);
					}

					if(HAL_UART_Receive_IT(&huart2,&huart2_buf,1)==HAL_OK)
            {
              huart2_rev_flag =1;
              }
            else
              {
                huart2_rev_flag =0;
              }
      }
      else if(UartHandle->Instance ==huart3.Instance)   //uart2
      {
    
      }
     else if(UartHandle->Instance ==huart6.Instance)   //uart6
      {
      }

}
uint32_t Ticks_counter =0;
uint32_t RunTicks_counter =0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//  uint32_t i;
	
      if(htim->Instance ==TIM2)
        {
                SIM_UART_Timeout++;
              if(SIM_UART_Timeout>Wait_times)
              {
                  HAL_TIM_Base_Stop_IT(&htim2);
                  SIM_UART_Timeout =0;
	    		  			printf("uart2: %s :end \r\n",SIM_DATA_BUF);
									GSM_Status_Check(SIM_DATA_BUF,SIM_Buf_length);
									//Delay_break =1;
                  memset(SIM_DATA_BUF,0,SIM_REV_BUF_MAX);
                  SIM_Buf_length =0;
                  HAL_TIM_Base_Stop_IT(&htim2);
              }
        }
      else if(htim->Instance ==TIM3)
			{
            LED_Flash_flag =1;
						HAL_TIM_Base_Start_IT(&htim3);
			}
			else if(htim->Instance ==TIM4)
			{
						 RunTicks_counter++;
						 if(RunTicks_counter>60*App_Pram_data->timestamp)
						 {
								RunTicks_counter =0;
						 		runticks =1;
								HAL_TIM_Base_Start_IT(&htim4);
						 }

			}
			else if(htim->Instance ==TIM5)
			{

				HAL_TIM_Base_Stop_IT(&htim5);

			}
  else if(htim->Instance ==TIM6)
  {
			if(alarm_time==0)
			{
				mbedtls_timing_alarmed = 1;
				HAL_TIM_Base_Stop(&htim6);
			}
			else
			{
				alarm_time--;
				HAL_TIM_Base_Start_IT(&htim6);
			}
   }
   else if(htim->Instance ==TIM7)
    {
				Ticks_counter++;
				if(Ticks_counter>86400)
					{
						Ticks_counter =0;
						systicks =1;
						HAL_TIM_Base_Stop_IT(&htim7);
          }
              
					
     }
}
void close_socket()
{
		  uint8_t CMD[100];
			Cline_Status_1=0;
			//Creat TCP 
			Delay_break=0;
			if(IOT_GSM)  //"SIM7020C"
			{
				sprintf((char *)CMD,"%s=%d\r\n",CSOCL,TCP_numb); //IPV4,TCP,IP
				HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
			}
		else
			{
				sprintf((char *)CMD,"%s=%d\r\n",CSOCL,TCP_numb); //IPV4,TCP,IP
				HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
			}
	    Sleep_break(500);

}

//void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
//{

// printf("Here is RTC alarma callback\n\r ");

//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
