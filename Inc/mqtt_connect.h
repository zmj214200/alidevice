/**
  ******************************************************************************
  * @file    mqtt_connect.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the headers of the mqtt_connect functions.
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
#ifndef _mqtt_connect_H
#define _mqtt_connect_H
#include "stm32f4xx_hal.h"

#include "W25QXX.h"
uint8_t MQTT_Conf( void );
uint8_t MQTT_Link(void);
uint8_t MQTT_sub_topic(void);
uint8_t MQTT_pub_attrib_topic(char *data,uint16_t len);
uint8_t MQTT_pub_OTAinfo_topic(char *data,uint16_t len);
uint8_t MQTT_pub_OTAProg_topic(char *data,uint16_t len);
uint8_t MQTT_pub_NTP_topic(char *data,uint16_t len);
uint8_t Read_CQS(void);
#endif  /* _mqtt_connect_H */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
