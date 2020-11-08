#include "Iot_config.h"
#include <stdint.h>
#include <string.h>
#include "stdio.h"
#include "mqtt_connect.h"
#include "Hmacsha1.h"

#include "main.h"

extern  App_Pram	 *App_Pram_data;
extern uint8_t  CMDR_OK ;
extern uint8_t Delay_break;
extern UART_HandleTypeDef huart2;
extern uint8_t   MQTT_Client_OK ;
extern uint8_t QMTOPEN_FLAG ;
extern uint8_t QMTCONN_Flag ;
extern uint8_t LED_Numb_Flag;

void MQTT_Server(char *mqtt_server,char *ProductKey) 
	{
		sprintf(mqtt_server,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",ProductKey);
	}
	
void MQTT_attrib_post(char *attribute_post_topic,char *ProductKey,char *DeviceName) 
	{
		sprintf(attribute_post_topic,"/sys/%s/%s/thing/event/property/post",ProductKey,DeviceName);
	}
	
	
void MQTT_attrib_Sub(char *attribute_sub_topic,char *ProductKey,char *DeviceName) 
	{
		sprintf(attribute_sub_topic,"/sys/%s/%s/thing/service/property/set",ProductKey,DeviceName);
	}
	
void MQTT_OTA_Sub(char *OTA_sub_topic,char *ProductKey,char *DeviceName) 
{
	
		sprintf(OTA_sub_topic,"/ota/device/upgrade/%s/%s",ProductKey,DeviceName);

}

void MQTT_OTA_info_pub(char *OTA_info_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(OTA_info_topic,"/ota/device/inform/%s/%s",ProductKey,DeviceName);
}

void MQTT_OTA_progress_pub(char *OTA_progress_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(OTA_progress_topic,"/ota/device/progress/%s/%s",ProductKey,DeviceName);
}

void MQTT_OTA_request_pub(char *OTA_request_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(OTA_request_topic,"/ota/device/request/%s/%s",ProductKey,DeviceName);
}

void MQTT_NTP_request_pub(char *NTP_request_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(NTP_request_topic,"/ext/ntp/%s/%s/request",ProductKey,DeviceName);
}

void MQTT_NTP_resp_sub(char *NTP_resp_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(NTP_resp_topic,"/ext/ntp/%s/%s/response",ProductKey,DeviceName);
}

// /sys/{productKey}/{deviceName}/thing/sub/register
// /sys/{productKey}/{deviceName}/thing/sub/register_reply
void MQTT_register_pub(char *register_pub_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(register_pub_topic,"/sys/%s/%s/thing/sub/register",ProductKey,DeviceName);
}

void MQTT_register_reply_sub(char *register_sub_topic,char *ProductKey,char *DeviceName) 
{
		sprintf(register_sub_topic,"/sys/%s/%s/thing/sub/register_reply",ProductKey,DeviceName);
}

 //////////////////////////////////////////////////////////////////////////////////////
//AT+QMTCFG="ALIAUTH",0,"a1XSjsAfwUn","LLxNet","fpF8mTlgP7uqmbdpa0mTxTfbtpwO4zFb"
/*
 calculate login secret key 
*/
uint8_t MQTT_Conf( void )
{
//	int i;
//  char buffer[512];// "clientIdmyclientdeviceName3325262728EBproductKeya1n7HqCJq4Ytimestamp789";
	
//	sprintf(buffer,"clientId%sdeviceName%sproductKey%stimestamp%d",App_Pram_data->clientId,App_Pram_data->Device_Name,App_Pram_data->ProductKey,App_Pram_data->timestamp);
	
//	hmac_sha1((unsigned char *)App_Pram_data->ProductSecret,strlen((const char *)App_Pram_data->ProductSecret),(unsigned char *)buffer,strlen((const char *)buffer),(unsigned char *)App_Pram_data->digest);
     
//  printf("加密前数据:%s\n加密后数据:", buffer);
     //9BBFD716EDF06A92FE7FDFE02BFFEC7A9BFEC7AF
//  for ( i = 0; i < sizeof(App_Pram_data->digest); i++)
  //  {
  //      printf("%02X", (unsigned int)(*(App_Pram_data->digest+i)));
  //  }   
  //  printf("\n");
	return 1;
}
//void MQTT_OTA_progress_pub(char *OTA_progress_topic,char *ProductKey,char *DeviceName) 

/*
  return OTA Progress
*/
uint8_t MQTT_pub_OTAProg_topic(char *data,uint16_t len)
{
		char CMD[1024];
		char pub_topic[256];
		MQTT_OTA_progress_pub(pub_topic,ProductKey_1,DeviceName_1) ;
		CMDR_OK =0;
		
		memset(CMD,0,sizeof(CMD));
		while(CMDR_OK==0)
		{Delay_break =0;
			if(IOT_GSM) 
			{
			//	AT+CMQPUB=0,"mytopic",1,0,0,8,"31323334"
				sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,\"%s\"\r\n",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,len,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
			else
			{
				//AT+QMTPUB=0,1,1,0,"/sys/a1n7HqCJq4Y/3325262728EB/thing/event/property/post","{params:{CurrentTemperature:22.2}}"
				sprintf(CMD,"%s=0,1,1,0,\"%s\",\"%s\"\r\n",MQTT_PUB,pub_topic,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
		Sleep_break(900);
	}
}

/*
 publish NTP topic
*/
uint8_t MQTT_pub_NTP_topic(char *data,uint16_t len)
{
		char CMD[1024];
		char pub_topic[256];
		MQTT_NTP_request_pub(pub_topic,ProductKey_1,DeviceName_1) ;
		CMDR_OK =0;
		
		memset(CMD,0,sizeof(CMD));
		while(CMDR_OK==0)
		{
			Delay_break =0;
			if(IOT_GSM) 
			{
			//	AT+CMQPUB=0,"mytopic",1,0,0,8,"31323334"
				//sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,%s\n\r",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,0,"31323334");
				sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,\"%s\"\r\n",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,len,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
			else
			{
				sprintf(CMD,"%s=0,1,1,0,\"%s\",\"%s\"\r\n",MQTT_PUB,pub_topic,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
		Sleep_break(900);
	}
}

/*
 publish attrib topic
*/
uint8_t MQTT_pub_attrib_topic(char *data,uint16_t len)
{
		char CMD[1024];
		char pub_topic[256];
		MQTT_attrib_post(pub_topic,ProductKey_1,DeviceName_1) ;
		CMDR_OK =0;
		
		memset(CMD,0,sizeof(CMD));
		while(CMDR_OK==0)
		{
			Delay_break =0;
			if(IOT_GSM) 
			{
			//	AT+CMQPUB=0,"mytopic",1,0,0,8,"31323334"
				//sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,%s\n\r",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,0,"31323334");
				sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,\"%s\"\r\n",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,len,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
			else
			{
				sprintf(CMD,"%s=0,1,1,0,\"%s\",\"%s\"\r\n",MQTT_PUB,pub_topic,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
		Sleep_break(900);
	}
}

/*
 publish OTA topic
*/
uint8_t MQTT_pub_OTAinfo_topic(char *data,uint16_t len)
{
		char CMD[1024];
		char pub_topic[256];
		MQTT_OTA_info_pub(pub_topic,ProductKey_1,DeviceName_1) ;
		CMDR_OK =0;
		
		memset(CMD,0,sizeof(CMD));
		while(CMDR_OK==0)
		{Delay_break =0;
			if(IOT_GSM) 
			{
			//	AT+CMQPUB=0,"mytopic",1,0,0,8,"31323334"
				sprintf(CMD,"%s=%d,\"%s\",%d,%d,%d,%d,\"%s\"\r\n",MQTT_PUB,0,pub_topic,MQTT_CQS,0,0,len,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
			else
			{
				//AT+QMTPUB=0,1,1,0,"/sys/a1n7HqCJq4Y/3325262728EB/thing/event/property/post","{params:{CurrentTemperature:22.2}}"
				sprintf(CMD,"%s=0,1,1,0,\"%s\",\"%s\"\r\n",MQTT_PUB,pub_topic,data);
				printf("%s\n\r",CMD);
				HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			}
		Sleep_break(900);
	}
}

/*
 subscribe topic
*/

uint8_t MQTT_sub_topic(void)
{
	char CMD[1024];
	char sub_topic[256];
	
	

  CMDR_OK =0;
	
	memset(CMD,0,sizeof(CMD));
	MQTT_attrib_Sub(sub_topic,ProductKey_1,DeviceName_1) ;
	while(CMDR_OK==0)
	{
		Delay_break =0;
		if(IOT_GSM) 
	{
		//	AT+CMQSUB=0,"mytopic",1
	  	sprintf(CMD,"%s=0,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
	else
	{
		sprintf(CMD,"%s=0,1,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
		printf("%s\n\r",CMD);
		HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
		Sleep_break(900);
	}
	
	
	memset(sub_topic,0,sizeof(sub_topic));
	MQTT_OTA_Sub(sub_topic,ProductKey_1,DeviceName_1);
	CMDR_OK =0;
	
	memset(CMD,0,sizeof(CMD));
	while(CMDR_OK==0)
	{
		Delay_break =0;
		if(IOT_GSM) 
	{
		//	AT+CMQSUB=0,"mytopic",1
	  	sprintf(CMD,"%s=0,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
	else
	{
	  	sprintf(CMD,"%s=0,1,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
		Sleep_break(900);
	}
	
	memset(sub_topic,0,sizeof(sub_topic));
	MQTT_register_reply_sub(sub_topic,ProductKey_1,DeviceName_1);
	CMDR_OK =0;
	memset(CMD,0,sizeof(CMD));
	while(CMDR_OK==0)
	{
		Delay_break =0;
		if(IOT_GSM) 
	{
		//	AT+CMQSUB=0,"mytopic",1
	  	sprintf(CMD,"%s=0,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
	else
	{
	  	sprintf(CMD,"%s=0,1,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
		Sleep_break(900);
	}
	
	memset(sub_topic,0,sizeof(sub_topic));
	MQTT_NTP_resp_sub(sub_topic,ProductKey_1,DeviceName_1);
	CMDR_OK =0;
	memset(CMD,0,sizeof(CMD));
	while(CMDR_OK==0)
	{
		Delay_break =0;
		if(IOT_GSM) 
	{
		//	AT+CMQSUB=0,"mytopic",1
	  	sprintf(CMD,"%s=0,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
	else
	{
	  	sprintf(CMD,"%s=0,1,\"%s\",%d\r\n",MQTT_SUB,sub_topic,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
	}
		Sleep_break(900);
	}
	
	
	return 1;
}




 /*MQTT Link*/
 /*连接MQTT服务器*/
uint8_t MQTT_Link(void)
{
	char CMD[1024];
	char Mqtt_server[256];
	CMDR_OK =0;
	
	
	MQTT_Server(Mqtt_server,ProductKey_1);
	
	while(CMDR_OK==0)
	{Delay_break =0;
		if(IOT_GSM) 
	{
  	sprintf(CMD,"%s=\"%s\",\"%d\",%d,%d\r\n",MQTT_NEW,Mqtt_server,MQTT_Port,MQTT_Timeout,MQTT_Buf);
		printf("%s\n\r",CMD);
		HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((const char *)CMD), 0xFFFF);
	}
	else
	{
//		AT+QMTCFG="ALIAUTH",0,"a1XSjsAfwUn","LLxNet","fpF8mTlgP7uqmbdpa0mTxTfbtpwO4zFb"
		sprintf(CMD,"%s=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",MQTT_ALICFG,ProductKey_1,DeviceName_1,DeviceSecret_1);
		printf("%s\n\r",CMD);
		HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((const char *)CMD), 0xFFFF);
	
	}
	Sleep_break(800);
	}
	CMDR_OK =0;
	
	memset(CMD,0,sizeof(CMD));
	QMTOPEN_FLAG =0;
	while((CMDR_OK==0)||(QMTOPEN_FLAG ==0))
	{
		
		if(IOT_GSM) 
	{Delay_break =0;
		//	AT+CMQALICFG=0,"a1n7HqCJq4Y","3325262728EB","RR0kLbS8t9UnL58qniwEl1wS055Yj1qh"
	  	sprintf(CMD,"%s=0,\"%s\",\"%s\",\"%s\"\r\n",MQTT_ALICFG,ProductKey_1,DeviceName_1,DeviceSecret_1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			Sleep_break(800);
	}
	else
	{
			//AT+QMTOPEN=0,"a1n7HqCJq4Y.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
		
			sprintf(CMD,"%s=0,\"%s\",%d\r\n",MQTT_NEW,Ali_Mqtt_server,MQTT_Port);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			HAL_Delay(15000);
	}
		
}
	CMDR_OK =0;
	QMTCONN_Flag=0;
	memset(CMD,0,sizeof(CMD));
	Delay_break =0;
	while((CMDR_OK==0)||(QMTCONN_Flag ==0))
	{
		if(IOT_GSM) 
	{Delay_break =0;
		//	AT+CMQALICON=0,600,1 
	  	sprintf(CMD,"%s=0,%d,%d\r\n",MQTT_CMQALICON,600,1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			Sleep_break(800);
	}
	else
	{
		//AT+QMTCONN=0,"clientexample"
			sprintf(CMD,"%s=0,\"%s\"\r\n",MQTT_CON,MQTT_client_id_1);
			printf("%s\n\r",CMD);
			HAL_UART_Transmit(&huart2, (uint8_t *)CMD, strlen((char *)CMD), 0xFFFF);
			HAL_Delay(15000);
	}
		
	}
	MQTT_Client_OK =1;	
	LED_Numb_Flag = LED_Numb_Flag|0x02;
	return 1;
}

uint8_t Read_CQS(void)
{
	char CMD[1024];
	
	while(CMDR_OK==0)
	{Delay_break =0;
	if(IOT_GSM)  //"SIM7020C"
	{
		sprintf(CMD,"%s\r\n",CSQ);
		HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
	}
	else
	{
		sprintf(CMD,"%s\r\n",CSQ);
		HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
	}
	Sleep_break(300);	
	}
	return 1;
}

uint8_t Mqtt_Close(void)
{
	char CMD[1024];
	
	while(CMDR_OK==0)
	{Delay_break =0;
	if(IOT_GSM)  //"SIM7020C"
	{
		sprintf(CMD,"%s=0\r\n",MQTT_DISCON);
		HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
	}
	else
	{
		sprintf(CMD,"%s=0\r\n",MQTT_DISCON);
		HAL_UART_Transmit(&huart2,(uint8_t *)CMD, strlen(CMD), 0xFFFF);
	}
	Sleep_break(300);	
	}
	return 1;


}

/////////////////////////////////////////////////////////////////////////////////

