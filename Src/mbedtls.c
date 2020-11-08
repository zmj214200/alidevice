/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : mbedtls.c
  * Description        : This file provides code for the configuration
  *                      of the mbedtls instances.
  ******************************************************************************
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
#include "mbedtls.h"

/* USER CODE BEGIN 0 */
#include "net_sockets.h"
#include "entropy.h"
#include "ctr_drbg.h"
#include "md5.h"
#include <string.h>
#include <stdio.h>

#include <stdlib.h> 
#include "main.h"



/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 2 */
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_ssl_session saved_session;
    mbedtls_x509_crt cacert;
/* USER CODE END 2 */

/* MBEDTLS init function */
void MX_MBEDTLS_Init(void)
{
   /**
  */
  /* USER CODE BEGIN 3 */
 /*
     * mbedtls config
     */

    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    memset(&saved_session, 0, sizeof( mbedtls_ssl_session));
    mbedtls_entropy_init( &entropy );

  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */

int os_get_random(unsigned char *buf, size_t len)
{
    int i, j;
    unsigned long tmp;
    srand(time(0)); 

    for (i = 0; i < ((len + 3) & ~3) / 4; i++) {
			
        tmp = rand();
         for (j = 0; j < 4; j++) {
            if ((i * 4 + j) < len) {
                buf[i * 4 + j] = (uint8_t)(tmp >> (j * 8));
            } else {
                break;
            }
        }
    }
 
    return 0;
}
/*
unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
		struct _hr_time *t = (struct _hr_time *) val;

    if( reset )
    {
        gettimeofday( &t->start, NULL );
        return( 0 );
    }
    else
    {
        unsigned long delta;
        struct timeval now;
        gettimeofday( &now, NULL );
        delta = ( now.tv_sec  - t->start.tv_sec  ) * 1000ul
              + ( now.tv_usec - t->start.tv_usec ) / 1000;
        return( delta );
    }
}

void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{


}
int mbedtls_timing_get_delay( void data )
{

  HAL_RTC_SetTime(&hrtc, &Times, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &Dates, RTC_FORMAT_BCD);
  HAL_RTC_GetTime(&hrtc, &Times, RTC_FORMAT_BCD);
  HAL_RTC_GetDate(&hrtc, &Dates, RTC_FORMAT_BCD);
  printf("Today is %2x year, %2x mouth, %2x date\n\r",Dates.Year,Dates.Month,Dates.Date);
  printf("now is %2x : %2x : %2x : %2x : %2x \n\r",Times.Hours,Times.Minutes,Times.Seconds,Times.SubSeconds,Times.SecondFraction);
  printf("App start ......\n");

}*/

/* USER CODE END 4 */

/**
  * @}
  */
 
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
