/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define SERVER_NAME "ota.iot-thing.cn-shanghai.aliyuncs.com"
#define SERVER_PORT 443
#define GET_REQUEST "GET "\
					"/ota/429012976cf46cae35b53858f0473e47/ck4xy72hf00063b7fcu1vv64w.bin?Expires=1578129656&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=XBFlq%2BY%2BQpZZLpto5Kf2opGZvTU%3D"\
					" HTTP/1.1\r\n"\
					"host: ota.iot-thing.cn-shanghai.aliyuncs.com\r\n"\
					"Accept: */*\r\n"\
	//		"Range:bytes=0-512\r\n\r\n"

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define Dev_addr       0x01
#define Reg_Start_addr       0x00
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_WDI_Pin GPIO_PIN_0
#define MCU_WDI_GPIO_Port GPIOC
#define GS_PSM_EINT_Pin GPIO_PIN_1
#define GS_PSM_EINT_GPIO_Port GPIOA
#define STM_STA_Pin GPIO_PIN_4
#define STM_STA_GPIO_Port GPIOA
#define SIM_CD_Pin GPIO_PIN_1
#define SIM_CD_GPIO_Port GPIOB
#define STM_RESET_Pin GPIO_PIN_10
#define STM_RESET_GPIO_Port GPIOB
#define STM_PWRKEY_Pin GPIO_PIN_11
#define STM_PWRKEY_GPIO_Port GPIOB
#define ERROR_Pin GPIO_PIN_12
#define ERROR_GPIO_Port GPIOB
#define RUN_Pin GPIO_PIN_13
#define RUN_GPIO_Port GPIOB
#define Link_Pin GPIO_PIN_14
#define Link_GPIO_Port GPIOB
#define SPI_NSS_Pin GPIO_PIN_6
#define SPI_NSS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

uint8_t Sleep_break(uint32_t Tmsec);
#pragma pack(1)
typedef struct  App_Pram
{
	char    			ProductKey[32];
	char    			ProductSecret[32];
	char    			Product_Id[50];
	char    			Device_Name[50];
	char          clientId[20];
	unsigned char digest[20];
	float			 		Hardware_vesion;
	char    			Up_Server_addr[1024];
	uint16_t   		Up_Server_addr_size;
	uint32_t   		Firmware_size;
	char	 	 			Firmware_sign[32];
	float			 		Firmware_Version;
	float         Firmware_Version_new;
	//////////////////////////////////////////////////
	uint32_t			Modbus_burrate;   //0: 9600 1: 4800 2: 57600 3: 115200
	uint8_t				Modbus_databit;   //0: 8bit 1:7bit
	uint8_t				Modbus_Stopbit;    // 0:1bit 1:2bit
	uint8_t				Modbus_Check;      //0: none 1:odd 2:even
	//////////////////////////////////////////////////
	uint8_t       NTPStart;         
	uint32_t			timestamp;    //设备数据 上报周期 分钟为单位
	//////////明柏设备////////////////////////////////////////////
	uint32_t      zsta;
	float Main_value_channel1;
	float Fu1_value_channel1;	
	float Fu2_Value_channel1;		
	uint8_t  channel1_en;
	float Main_value_channel2;
	float Fu1_value_channel2;	
	float Fu2_Value_channel2;		
	uint8_t  channel2_en;
	float Main_value_channel3;
	float Fu1_value_channel3;	
	float Fu2_Value_channel3;		
	uint8_t  channel3_en;
	float Main_value_channel4;
	float Fu1_value_channel4;	
	float Fu2_Value_channel4;		
	uint8_t  channel4_en;
	float Current_set_1;
	uint8_t  Current1_en;
	float Current_set_2;
	uint8_t  Current2_en;
	float Current_set_3;
	uint8_t  Current3_en;
	float Current_set_4;	
	uint8_t  Current4_en;
	
	float Relax1_Set_Value;
	uint8_t  Relax1_en;
	float Relax2_Set_Value;
	uint8_t  Relax2_en;
	float Relax3_Set_Value;
	uint8_t  Relax3_en;
	float Relax4_Set_Value;
	uint8_t  Relax4_en;
	float Relax5_Set_Value;
	uint8_t  Relax5_en;
	float Relax6_Set_Value;
	uint8_t  Relax6_en;
	float Relax7_Set_Value;
	uint8_t  Relax7_en;
	float Relax8_Set_Value;	
	uint8_t  Relax8_en;
	
	uint16_t switch1;
	uint8_t  switch1_en;
	uint16_t switch2;
	uint8_t  switch2_en;	
	uint16_t switch3;
	uint8_t  switch3_en;
	uint16_t switch4;
	uint8_t  switch4_en;
	uint16_t switch5;
	uint8_t  switch5_en;
	uint16_t switch6;
	uint8_t  switch6_en;
	uint16_t switch7;
	uint8_t  switch7_en;
	uint16_t switch8;
	uint8_t  switch8_en;
	uint16_t switch9;
	uint8_t  switch9_en;
	uint16_t switch10;
	uint8_t  switch10_en;
	uint16_t switch11;
	uint8_t  switch11_en;
	uint16_t switch12;
	uint8_t  switch12_en;
	uint32_t  FUN_Code;
	//////////////////////////////////////////////////////////
	uint8_t       downloading ;  //0 未开始下载   1正在下载中
	uint32_t			downloadoffset;
	uint32_t			write_offset;
	uint8_t    		Write_Flag;
	
}App_Pram;

#pragma pack()
#pragma pack(1)
typedef struct  Boot_pram
{
	float			 Hardware_vesion;
	float			 Firmware_Version;
	uint8_t    App_updata_Ready;          //升级APP，将SIP flash APP 复制到 芯片内部Flash.
  uint8_t    Write_Flag;
}Boot_pram;
#pragma pack()
#pragma pack(1)
typedef struct{
	//运行内部参数
	size_t content_len;
	size_t body_processed;
	/*header*/
	uint8_t* phead;
	uint16_t head_len;
}tcpc_httpc_t;
#pragma pack()
#pragma pack(1)
typedef struct{
   int tm_sec;         
   int tm_min;        
   int tm_hour;        
   int tm_mday;      
   int tm_mon;         
   int tm_year;       
   int tm_wday;        
   int tm_yday;       
   int tm_isdst;     
}time_1;
#pragma pack()

void StringtoHex(char *datain,uint16_t length,char *dataout,uint16_t length_out);
uint16_t HextoString(char *datain,uint16_t length,char *dataout);
unsigned long mktime1(unsigned int year, unsigned int mon,unsigned int day, unsigned int hour,unsigned int min, unsigned int sec);
uint8_t init_SIM(void);
void Pow_up_sim();
void close_socket();
void REST_SIM();
uint8_t Sleep_break_net(uint32_t Tmsec);
void feed_dog();
#define HTTP_HEAD_SIZE	1500
#define MIN(a,b) ( (a) < (b) ? (a) : (b) )

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
