/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : timing_alt.c
  * @author  MCD Application Team
  * @brief   mbedtls alternate timing functions implementation.
  *          mbedtls timing API is implemented using the CMSIS-RTOS v1/v2 API
  *          this file has to be reamed to timing_alt.c and copied under
  *          the project tree.
  ******************************************************************************
  */
  /* USER CODE END Header */
  /*
  * Licensed under the Apache License, Version 2.0 (the "License"); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * http://www.apache.org/licenses/LICENSE-2.0
  */

#include "timing_alt.h"

/* include the appropriate header file */
#include "stm32f4xx_hal.h"
//#include "cmsis_os.h"
#include "main.h"
#include <time.h>

extern  RTC_HandleTypeDef hrtc;
extern  RTC_TimeTypeDef  Times  ;
extern  RTC_DateTypeDef  Dates  ;
extern  TIM_HandleTypeDef htim6;

uint32_t alarm_time;

struct tm  local_time_mbed;
time_t     Sys_times_mbed;

typedef struct os_timer_cb *osTimerId;

struct _hr_time
{
    uint32_t elapsed_time;
	};
volatile int mbedtls_timing_alarmed = 0;

static uint8_t timer_created = 0;

static osTimerId timer;
	
	
unsigned long mktime1(unsigned int year, unsigned int mon,unsigned int day, unsigned int hour,unsigned int min, unsigned int sec)
{
    if (0 >= (int) (mon -= 2)) { 
         mon += 12;     
         year -= 1;
    }

    return (((
             (unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
             year*365 - 719499
          )*24 + hour 
       )*60 + min 
    )*60 + sec; 
}
/*
static void osTimerCallback(void const *argument)
{
  UNUSED(argument);
  mbedtls_timing_alarmed = 1;
  osTimerStop(timer);
}*/
unsigned long mbedtls_timing_hardclock( void )
{
   /* retrieve the CPU cycles using the Cortex-M DWT->CYCCNT register
    * avaialable only starting from CM3
    */

#if (__CORTEX_M >= 0x03U)
    static int dwt_started = 0;
    if( dwt_started == 0 )
    {
      dwt_started = 1;
      /* Enable Tracing */
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if (__CORTEX_M == 0x07U)
        /* in Cortex M7, the trace needs to be unlocked
         * via the DWT->LAR register with 0xC5ACCE55 value
         */
        DWT->LAR = 0xC5ACCE55;
#endif
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

      /* Reset counter */
      DWT->CYCCNT = 0;
    }

    return (unsigned long)DWT->CYCCNT;
#else
    return 0;
#endif
}

unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
  unsigned long delta;
  uint32_t offset;
  struct _hr_time *t = (struct _hr_time *) val;
				HAL_RTC_GetTime(&hrtc, &Times, RTC_FORMAT_BIN);
			  HAL_RTC_GetDate(&hrtc, &Dates, RTC_FORMAT_BIN);
				local_time_mbed.tm_year= Dates.Year+100;
				local_time_mbed.tm_mon= Dates.Month-1;
				local_time_mbed.tm_mday = Dates.Date;
				local_time_mbed.tm_hour  = Times.Hours;
				local_time_mbed.tm_min = Times.Minutes;
				local_time_mbed.tm_sec = Times.Seconds;
				local_time_mbed.tm_isdst =0;
				printf("now time:%s \r\n",asctime(&local_time_mbed));
				Sys_times_mbed = mktime(&local_time_mbed);
//	offset = osKernelSysTick();
//	offset = mktime1(Dates.Year,Dates.Month,Dates.Date,Times.Hours,Times.Minutes,Times.Seconds);
	
  if( reset )
  {
    t->elapsed_time = Sys_times_mbed;
    return( 0 );
  }

  delta = Sys_times_mbed - t->elapsed_time;
  return( delta );

}

void mbedtls_set_alarm( int seconds )
{
	alarm_time = seconds*1000;
  mbedtls_timing_alarmed = 0;

	HAL_TIM_Base_Start_IT(&htim6);
}

void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{
  mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

  ctx->int_ms = int_ms;
  ctx->fin_ms = fin_ms;

  if( fin_ms != 0 )
    mbedtls_timing_get_timer( &ctx->timer, 1 );
}

int mbedtls_timing_get_delay( void *data )
{
  mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
  unsigned long elapsed_ms;

  if( ctx->fin_ms == 0 )
    return( -1 );

  elapsed_ms = mbedtls_timing_get_timer( &ctx->timer, 0 );

  if( elapsed_ms >= ctx->fin_ms )
    return( 2 );

  if( elapsed_ms >= ctx->int_ms )
    return( 1 );

  return( 0 );
}
