/*********************************************************************************************************
*
* File                : Hmac-sha1.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 5.15
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2015, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Hmacsha1_H
#define __Hmacsha1_H
#ifdef __cplusplus
 extern "C" {
#endif 
/* Includes ------------------------------------------------------------------*/


#define MAX_MESSAGE_LENGTH 2048//最大消息长度


unsigned long int ft(
                    int t,
                    unsigned long int x,
                    unsigned long int y,
                    unsigned long int z
                    );
 
int get_testcase(   int test_case,
                    unsigned char *plaintext,
                    unsigned char *key,
                    int *key_length_ptr);

void sha1   (
            unsigned char *message,
            int message_length,
            unsigned char *digest
            );
void hmac_sha1(
                unsigned char *key,
                int key_length,
                unsigned char *data,
                int data_length,
                unsigned char *digest
                );
/**
  * @}
  */
  
/** @defgroup W25Q128FV_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 
      
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __W25Qx_H */


