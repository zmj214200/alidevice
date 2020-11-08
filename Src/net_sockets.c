/* USER CODE BEGIN Header */
/** 
  ******************************************************************************
  * File Name       : net_sockets.c.h
  * Description     : TCP/IP or UDP/IP networking empty functions
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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "mbedtls/net_sockets.h"

//#include "ethernetif.h"
#include "stm32f4xx_hal.h"

#include "main.h"
/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN INCLUDE */
#include "Iot_config.h"
extern uint8_t Rev_flag;
extern uint8_t NetOpen_Status_1;
extern uint8_t Delay_break;
extern uint8_t Cline_Status_1;
extern uint8_t TCP_numb;
extern UART_HandleTypeDef huart2;
extern uint8_t Send_OK;
extern char Lan_rev_data[SIM_REV_BUF_MAX];
extern uint32_t  Lan_rev_lenth;
extern uint8_t huart2_buf;
extern uint8_t    NetOpen_Status_1;
extern uint8_t  clinent_server_error;
extern uint8_t  Delay_break_net;
extern uint8_t   end_cmd[2];
/* USER CODE END INCLUDE */
//static struct netif netif;


/* USER CODE BEGIN VARIABLES */

/* USER CODE END VARIABLES */
/*
 * Initialize TCP/IP stack and get a dynamic IP address.
 */
void mbedtls_net_init( mbedtls_net_context *ctx )
{
/* USER CODE BEGIN 0 */
	
  	uint8_t CMD[100];
		NetOpen_Status_1=0;
		while(NetOpen_Status_1==0)
	 {
			//Creat TCP 
			Delay_break=0;
			if(IOT_GSM)  //"SIM7020C"
			{
				sprintf((char *)CMD,"%s=%d,%d,%d\r\n",CSOC,1,1,1); //IPV4,TCP,IP
				HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
			}
		else
			{
					Delay_break=1;
					NetOpen_Status_1=1;
			}
	    Sleep_break(800);

			if(NetOpen_Status_1==1)break;
	 }
/* USER CODE END 0 */
printf("LAN init OK \n\r");
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect( mbedtls_net_context *ctx, const char *host, const char *port, int proto )
{
/* USER CODE BEGIN 1 */
	
    uint8_t CMD[100];
	if(NetOpen_Status_1)
	{
		Cline_Status_1 =0;
		while(Cline_Status_1==0)
	 {
		 printf("connectting....\r\n ");
			
			if(IOT_GSM)  //"SIM7020C"
			{Delay_break=0;
					sprintf((char *)CMD,"%s=%d,%s,\"%s\"\r\n",CSOCON,TCP_numb,port,host); //
				  printf("%s\n",CMD);
					HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
			}
		else
			{
				Delay_break_net=0;
				//	AT+QIOPEN=1,0,"TCP","220.180.239.212",8062,1234,0
				  sprintf((char *)CMD,"%s=1,0,\"TCP\",\"%s\",%s,1234,1,0\r\n",CSOCON,host,port); //
					printf("%s\n",CMD);
					HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
					HAL_UART_Transmit(&huart2, end_cmd, strlen((char *)end_cmd), 0xFFFF);
			}
			 if(IOT_GSM)
			 {
				Sleep_break(800);
			 }
	    else
			{
				Sleep_break_net(5000);
			}
		//	HAL_Delay(5000);
			if(Cline_Status_1==1)break;
		}
	 	 printf("client NET OK\n\r");
     return 0;
	}
	else
	{
		 	 printf("client NET Error\n\r");
				return 0;
	}
		
	 

/* USER CODE END 1 */
}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind( mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto )
{
  int ret = 0;
/* USER CODE BEGIN 2 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
/* USER CODE END 2 */
  return ret;
}

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{
/* USER CODE BEGIN 3 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0;
/* USER CODE END 3 */
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
/* USER CODE BEGIN 4 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0;
/* USER CODE END 4 */
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
/* USER CODE BEGIN 5 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0;
/* USER CODE END 5 */
}

/*
 * Portable usleep helper
 */
void mbedtls_net_usleep( unsigned long usec )
{
/* USER CODE BEGIN 6 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
/* USER CODE END 6 */
}

/*
 * Read at most 'len' characters
 */
uint32_t  read_numb=0;
uint32_t  remain_numb=0;
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
	uint32_t i;
	uint16_t time_out=0;
/* USER CODE BEGIN 7 */
// printf("rev fun Lan_rev_lenth = %d; len= %d ;read_numb =%d ; remain_numb =%d ;\r\n",Lan_rev_lenth,len,read_numb,remain_numb);
	
 while(Rev_flag==0)
 {
	 time_out++;
	 if(time_out>100)
	 {
			time_out =0;
		  break;
	 }
	 feed_dog();
	HAL_Delay(500);
	 feed_dog();
 }
 
 if(IOT_GSM)
 {
  
		if(Lan_rev_lenth>0)
		{
				if((Lan_rev_lenth-read_numb)>=len*2)
			{
					for(i=0;i<len;i++)
					{
							if(Lan_rev_data[2*i+read_numb]<0x40)
							{
									if(Lan_rev_data[2*i+1+read_numb]<0x40)
									{
											buf[i]=(Lan_rev_data[2*i+read_numb]-0x30)*0x10+(Lan_rev_data[2*i+1+read_numb]-0x30);
										}
										else
										{
											buf[i]=(Lan_rev_data[2*i+read_numb]-0x30)*0x10+(Lan_rev_data[2*i+1+read_numb]-0x41+10);
											}
									}
									else
									{
											if(Lan_rev_data[2*i+1+read_numb]<0x40)
											{
													buf[i]=(Lan_rev_data[2*i+read_numb]-0x41+10)*0x10+(Lan_rev_data[2*i+1+read_numb]-0x30);
													}
												else
												{
													buf[i]=(Lan_rev_data[2*i+read_numb]-0x41+10)*0x10+(Lan_rev_data[2*i+1+read_numb]-0x41+10);
													}
										}
			
							}
						read_numb = read_numb + len*2;// Lan_rev_lenth/2;
					}
				if(read_numb==Lan_rev_lenth)
				{
						read_numb =0;
						Lan_rev_lenth =0;
						Rev_flag =0;
						memset(Lan_rev_data,0,Lan_rev_lenth);
					}
					printf("\r\n rev %d OK\r\n",len);

					return len;
			}
			else
			{
					return 0;
				}
 
		}
	else
		{
			if(Lan_rev_lenth>0)
				{
						if((Lan_rev_lenth-read_numb)>=len)
						{
								for(i=0;i<len;i++)
								{
									buf[i]=Lan_rev_data[i+read_numb];
									}
								read_numb = read_numb + len;// Lan_rev_lenth/2;
							}
						if(read_numb==Lan_rev_lenth)
						{
							read_numb =0;
							Lan_rev_lenth =0;
							Rev_flag =0;
							memset(Lan_rev_data,0,Lan_rev_lenth);
							}
						printf("\r\n rev %d OK\r\n",len);
						return len;
					}
				else
				{
						return 0;
				}
			}
/* USER CODE END 7 */
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
/* USER CODE BEGIN 8 */
 printf("mbedtls_net_recv_timeout\n\r");
  return 0;  
/* USER CODE END 8 */
}

static int net_would_block( const mbedtls_net_context *ctx )
{
/* USER CODE BEGIN 9 */
  printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0; 
/* USER CODE END 9 */
}

/*
 * Write at most 'len' characters
	#define SEND  					"AT+CSOSEND"
AT+CSOSEND=0,10,"122121212121"
AT+CSOSEND=0,0,¡°hello¡±
#define SEND  					"AT+CIPSEND"
AT+CIPSEND
> hello TCP serve
0x1A½áÊø

 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
/* USER CODE BEGIN 10 */
  uint8_t CMD[2048];
	char   hex_buf[2048];
	memset(hex_buf,0,2048);
	Send_OK=0;
	uint32_t i;
	uint32_t len_temp =len;
	uint8_t  temp=0;
	memset(CMD,0,2048);
	
	if(len_temp>512)
	{
		while(len_temp>0)
		{
			if(len_temp>512)
			{
				printf("len_temp>256\r\n");
				for(i=0;i<512;i++)
				{
						if((buf[i+temp*512]/0x10)>9)
						{
							hex_buf[i*2] = 0x41+(buf[i+temp*512]/0x10 -10);
						}	
						else
						{
								hex_buf[i*2] = 0x30+buf[i+temp*512]/0x10;
							}
						if((buf[i+temp*512]%0x10)>9)
						{
								hex_buf[i*2+1] = 0x41+(buf[i+temp*512]%0x10-10);
							}	
						else
							{
									hex_buf[i*2+1] = 0x30+buf[i+temp*512]%0x10;
								}
					}
					Send_OK = 0;
					while(Send_OK==0)
					{
						if(IOT_GSM) 
						{
						Delay_break=0;
						sprintf((char *)CMD,"%s=%d,%d,\"%s\"\r\n",SEND,TCP_numb,512*2,hex_buf);
					//	printf("%s\n",CMD);
						HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
						HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
						memset(CMD,0,sizeof(CMD));
						Sleep_break(800);
						if(NetOpen_Status_1==0){
							return 0;
						}
						if(Send_OK==1)break;
						}
						else
						{
							Delay_break=0;                               //char *buf, size_t len
							//AT+QISENDEX=0,5,3031323334
							sprintf((char *)CMD,"%s=%d,%d,%s\r\n",SENDhex,0,512,hex_buf);
					//		printf("%s\n",CMD);
							HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
							HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
							memset(CMD,0,sizeof(CMD));
							if(NetOpen_Status_1==0){
								return 0;
								}
							Sleep_break(800);
							if(clinent_server_error)
								{
									return 0;
								}
								if(Send_OK==1)break;
						}
					}
					len_temp =len_temp -512;
					temp++;
					memset(hex_buf,0,sizeof(hex_buf));
			}
			else
			{
				printf("len_temp<1024\r\n");
				for(i=0;i<len_temp;i++)
				{
						if((buf[i+(len-len_temp)]/0x10)>9)
						{
							hex_buf[i*2] = 0x41+(buf[i+(len-len_temp)]/0x10 -10);
						}	
						else
						{
								hex_buf[i*2] = 0x30+buf[i+(len-len_temp)]/0x10;
							}
						if((buf[i+(len-len_temp)]%0x10)>9)
						{
								hex_buf[i*2+1] = 0x41+(buf[i+(len-len_temp)]%0x10-10);
							}	
						else
							{
									hex_buf[i*2+1] = 0x30+buf[i+(len-len_temp)]%0x10;
								}
					}
					Send_OK = 0;
					while(Send_OK==0)
					{
						if(IOT_GSM) 
						{
						Delay_break=0;
						sprintf((char *)CMD,"%s=%d,%d,\"%s\"\r\n",SEND,TCP_numb,len_temp*2,hex_buf);
				//		printf("%s\n",CMD);
						HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
						HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
						memset(CMD,0,sizeof(CMD));
						Sleep_break(800);
						if(NetOpen_Status_1==0){
							return 0;
						}
						if(Send_OK==1)break;
						}
						else
						{
							Delay_break=0;                               //char *buf, size_t len
							//AT+QISENDEX=0,5,3031323334
							sprintf((char *)CMD,"%s=%d,%d,%s\r\n",SENDhex,0,len_temp,hex_buf);
						//	printf("%s\n",CMD);
							HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
							HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
							memset(CMD,0,sizeof(CMD));
							if(NetOpen_Status_1==0){
								return 0;
								}
							Sleep_break(800);
							if(clinent_server_error)
								{
									return 0;
								}
								if(Send_OK==1)break;
						}
					}
					len_temp =0;
					memset(hex_buf,0,sizeof(hex_buf));
			}
		}
	}
	else
	{
		
				for(i=0;i<len;i++)
				{
						if((buf[i]/0x10)>9)
						{
							hex_buf[i*2] = 0x41+(buf[i]/0x10 -10);
						}	
						else
						{
								hex_buf[i*2] = 0x30+buf[i]/0x10;
							}
						if((buf[i]%0x10)>9)
						{
								hex_buf[i*2+1] = 0x41+(buf[i]%0x10-10);
							}	
						else
							{
									hex_buf[i*2+1] = 0x30+buf[i]%0x10;
								}
					}
					Send_OK = 0;
					while(Send_OK==0)
					{
						if(IOT_GSM) 
						{
							Delay_break=0;                               //char *buf, size_t len
							sprintf((char *)CMD,"%s=%d,%d,\"%s\"\r\n",SEND,TCP_numb,len*2,hex_buf);
							printf("%s\n",CMD);
						//	HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
							HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
							memset(CMD,0,sizeof(CMD));
							if(NetOpen_Status_1==0){
								return 0;
								}
							Sleep_break(800);
						//	if(clinent_server_error==1)
							//	{
							//		return 0;
							//	}
								if(Send_OK==1)break;
						}
						else
						{
							Delay_break=0;                               //char *buf, size_t len
							//AT+QISENDEX=0,5,3031323334
							sprintf((char *)CMD,"%s=%d,%d,%s\r\n",SENDhex,0,len,hex_buf);
						//	printf("%s\n",CMD);
							HAL_UART_Receive_IT(&huart2,&huart2_buf,1);
							HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
							memset(CMD,0,sizeof(CMD));
							if(NetOpen_Status_1==0){
								return 0;
								}
							Sleep_break(800);
							if(clinent_server_error)
								{
									return 0;
								}
								if(Send_OK==1)break;
						}

					}
	}
		printf("send data OK \n\r");
  	return len;
/* USER CODE END 10 */
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free( mbedtls_net_context *ctx )
{
/* USER CODE BEGIN 11 */
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
			{  //AT+QICLOSE=0
				sprintf((char *)CMD,"%s=%d\r\n",CSOCL,TCP_numb); //IPV4,TCP,IP
				HAL_UART_Transmit(&huart2, CMD, strlen((char *)CMD), 0xFFFF);
			}
	    Sleep_break(500);
  printf ("Close Net AT!\n");
/* USER CODE END 11 */
 }

