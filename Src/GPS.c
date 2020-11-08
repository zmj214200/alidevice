#include "main.h"
#include "Iot_config.h"
#include "GPS.h"

extern UART_HandleTypeDef huart2;
extern uint8_t    RETURN_OK; 
 uint8_t init_GPS(void)
 {
	  char CMD[256];
          memset(CMD,0,256);
                RETURN_OK=0;
		/*Close ATE*/
		 while(RETURN_OK ==0)
		{

				sprintf(CMD,"%s\n\r","AT+QGNSSC=1");
				HAL_UART_Transmit(&huart2,(uint8_t *)CMD, sizeof("AT+QGNSSC=1")+2, 0xFFFF);
                                HAL_Delay(10000);
                  }
          return 0;
 }
 uint8_t Read_GPS(void)
 {
	  char CMD[256];
     memset(CMD,0,256);
     RETURN_OK=0;
		/*Close ATE*/
		 while(RETURN_OK ==0)
		{

				sprintf(CMD,"%s\n\r","AT+QGNSSRD=\"NMEA/RMC\"");
				HAL_UART_Transmit(&huart2,(uint8_t *)CMD, sizeof("AT+QGNSSRD=\"NMEA/RMC\"")+2, 0xFFFF);
                                HAL_Delay(10000);
                  }
          return 0;
 }