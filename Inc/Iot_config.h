/**
  ******************************************************************************
  * @file    config.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the headers of the common functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _Iot_config_H
#define _Iot_config_H
#include <stdint.h>  

/* Includes ------------------------------------------------------------------*/
#define Firmware_local_addr        0x08020000
#define Boot_pram_addr             0x080C0000
#define App_pram_addr              0x080E0000

#define Firmware_SPI_flash_addr    0x00010000
#define App_SPI_pram_addr          0x00001000
#define Boot_SPI_pram_addr         0x00000000

#define https_test_server          "58.211.191.76"

#define Hversion										1.0
#define Fversion										1.0

#define Boot_Hversion										1.0
#define Boot_Fversion										1.0
#define Mqtt_version										1.0

#define IOT_GSM                    	1
#define GPS													0

#define ProductKey_1                     	"a1e5ODXUMua"
#define DeviceSecret_1                  	"pVjonGjKIIH5HGmCLElhLE1uOXBruEa2"
#define Product_Id_1                      "5030548"
#define DeviceName_1                      "MB00001"

#define MQTT_client_id_1					   			"myclient"
#define timestamp_1                       1   
#define Ali_Mqtt_server										"iot-as-mqtt.cn-shanghai.aliyuncs.com"
    //password  9BBFD716EDF06A92FE7FDFE02BFFEC7A9BFEC7AF
#define Https_SERVER									"ota.iot-thing.cn-shanghai.aliyuncs.com"
#define Https_SERVER_NAME 						"https://ota.iot-thing.cn-shanghai.aliyuncs.com/ota/429012976cf46cae35b53858f0473e47/ck4z2g44f00063b7gx0klyftx.bin?Expires=1578287902&OSSAccessKeyId=cS8uRRy54RszYWna&Signature=EddiZnUG6TW%2BnNxZeWFEhd2ibgg%3D"
#define MQTT_Port											1883
#define Https_Port										443
#define MQTT_Timeout									12000
#define MQTT_Buf											1024
#define SIM_REV_BUF_MAX						 		1024*10
#define MQTT_CQS									 		1


#define NTP_server                    "ntp5.aliyun.com"


#if (IOT_GSM ==1)
	#define ATE							"ATE0"
	#define AT							"AT"
	#define CPIN						"AT+CPIN?"
	#define CSQ							"AT+CSQ"
	#define CGCONTRDP				"AT+CGCONTRDP"
	#define DNS							"AT+CDNSGIP"
	#define CSOC						"AT+CSOC"     //Creat socket
	#define CSOCL 					"AT+CSOCL"    //Close socket	
	#define CSOCON					"AT+CSOCON"   //Connect Socket
	#define SEND  					"AT+CSOSEND"
#define SENDhex  					"AT+QISENDEX"
	#define MQTT_NEW  			"AT+CMQNEW"
  #define MQTT_ALICFG			"AT+CMQALICFG"
	#define MQTT_CMQALICON	"AT+CMQALICON"
	#define MQTT_CON  			"AT+CMQCON"
	#define MQTT_DISCON			"AT+CMQDISCON"
	#define MQTT_SUB  			"AT+CMQSUB"
	#define MQTT_UNSUB 			"AT+CMQUNSUB"
	#define MQTT_PUB			  "AT+CMQPUB"
#else 
	#define ATE							"ATE0"
	#define AT							"AT"
	#define CPIN						"AT+CPIN"
	#define CSQ							"AT+CSQ"
	#define DNS							"AT+CDNSGIP"
	#define CSOC						"AT+CSOC"     //Creat socket  
	#define CGCONTRDP				"AT+CGCONTRDP"
	#define CSOCON					"AT+QIOPEN"     //Connect Socket
	#define CSOCL 					"AT+QICLOSE"    //Close socket
	#define SEND  					"AT+QISEND"
	#define SENDhex  				"AT+QISENDEX"
	#define MQTT_NEW  			"AT+QMTOPEN"
  #define MQTT_ALICFG			"AT+QMTCFG"
	#define MQTT_CMQALICON	"AT+CMQALICON"
	#define MQTT_CON  			"AT+QMTCONN"
	#define MQTT_DISCON			"AT+QMTDISC"
	#define MQTT_SUB  			"AT+QMTSUB"
	#define MQTT_UNSUB 			"AT+QMTUNS"
	#define MQTT_PUB			  "AT+QMTPUB"
#endif 
//////////////////define configure keyword
#define   Current_Limt   	"Current_Limt"
#define   PowerSwitch   	"PowerSwitch"
#define   DeviceTime    	"DeviceTime"
#define   NTP_Start    		"NTP_Start"
#define   modbus_Brate    "modbus_Brate"
#define   databit    			"databit"
#define   stopbit    			"stopbit"
#define   Checkbit    		"Checkbit"
#define   reflash_time    		"reflash_time"

#define   Channel1_enable				"Channel1_enable"
#define   Channel2_enable				"Channel2_enable"
#define   Channel3_enable				"Channel3_enable"
#define   Channel4_enable				"Channel4_enable"
#define   Current_set1    		"Current_set1"

#define   Current_set2    		"Current_set2"

#define   Current_set3    		"Current_set3"

#define   Current_set4    		"Current_set4"

#define   Relax_Set_Value1    		"Relax1_Set_Value1"

#define   Relax_Set_Value2    		"Relax2_Set_Value2"

#define   Relax_Set_Value3   	  	"Relax3_Set_Value3"

#define   Relax_Set_Value4    		"Relax4_Set_Value4"

#define   Relax_Set_Value5    		"Relax5_Set_Value5"

#define   Relax_Set_Value6    		"Relax6_Set_Value6"

#define   Relax_Set_Value7    		"Relax7_Set_Value7"

#define   Relax_Set_Value8    		"Relax8_Set_Value8"

#define   switch_1    							"switch1"

#define   switch_2    							"switch2"

#define   switch_3   	  						"switch3"

#define   switch_4    							"switch4"

#define   switch_5    							"switch5"

#define   switch_6   	  						"switch6"

#define   switch_7    							"switch7"

#define   switch_8    							"switch8"

#define   switch_9   	  						"switch9"

#define   switch_10    							"switch10"

#define   switch_11    							"switch11"

#define   switch_12   	  						"switch12"


#define   FUN_Code_set   	  						"FUN_Code"

#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
