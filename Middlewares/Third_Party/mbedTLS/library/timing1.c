
#include "timing.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "stm32f4xx_hal_rtc.h"
#include <stdint.h>
extern RTC_HandleTypeDef hrtc;


static inline unsigned long mktime (unsigned int year, unsigned int mon,unsigned int day, unsigned int hour,unsigned int min, unsigned int sec)
        {    
		if (0 >= (int) (mon -= 2)) 
		{         
				mon += 12;           
				year -= 1;   
                }    
				return ((((unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +year*365 - 719499 )*24 + hour )*60 + min )*60 + sec; /* finally seconds */
	}


unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
	RTC_TimeTypeDef  mbedtls_Times  ;
	RTC_DateTypeDef  mbedtls_Dates  ;
	HAL_RTC_GetTime(&hrtc, &mbedtls_Times, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &mbedtls_Dates, RTC_FORMAT_BIN);
        
        return mktime (mbedtls_Dates.Year + 100, mbedtls_Dates.Month-1,mbedtls_Dates.Date, mbedtls_Times.Hours-8,mbedtls_Times.Minutes, mbedtls_Times.Seconds);

}

/*

Set delays to watch
*/
void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{
    
}
/*

Get number of delays expired
*/
int mbedtls_timing_get_delay( )
{
  
    return  0;
}
/*timing.c 替换结束 */
