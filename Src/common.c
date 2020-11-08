/**
  ******************************************************************************
  * @file    IAP/src/common.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the common functions.
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

/** @addtogroup IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/

#include "common.h"



/* 外部定义变量          -----------------------------------------------------------*/

//读取指定地址
void FLASH_addr_Read(uint32_t read_addr,uint32_t readbuf_addr,uint16_t len)
{
  uint8_t dataIndex1;
   for(dataIndex1=0;dataIndex1<len;dataIndex1++)
  {

      *(uint32_t *)(readbuf_addr+dataIndex1) = *(uint32_t *)(read_addr+dataIndex1); 
  }
 
}


void FLASH_Read(uint32_t read_addr,uint8_t* readbuf,uint16_t len)
{
  uint32_t dataIndex1;
  uint32_t Flash_addr =read_addr;

   for(dataIndex1=0;dataIndex1<len;dataIndex1++)
  {
      *(uint32_t *)(readbuf+dataIndex1) = *(__IO uint32_t*)(Flash_addr+dataIndex1); 
  }
 
}

void delay_flash(uint32_t delay)
{
	uint32_t  i;
	for(i=0;i<delay;i++);
}
 // App_local_addr  0x08020000   Sector 5  Sector numb 5   "5 6 7 8 9"
void Erase_App(void)
{
	 HAL_StatusTypeDef rc =HAL_ERROR;
	 while(rc!=HAL_OK)
    {
      rc = HAL_FLASH_Unlock();
    }
		
    FLASH_EraseInitTypeDef f;
    
    f.TypeErase = FLASH_TYPEERASE_SECTORS;
	
    f.Sector = 5;            //
		
		f.NbSectors =5;          //
		
		f.VoltageRange =FLASH_VOLTAGE_RANGE_2;
 
    uint32_t PageError = 0;
 
    HAL_FLASHEx_Erase(&f, &PageError);
		
		HAL_FLASH_Lock();//
}

void Write_App(uint32_t write_addr,uint8_t* writebuf,uint32_t len)
{
		HAL_StatusTypeDef rc =HAL_ERROR;
		while(rc!=HAL_OK)
    {
      rc = HAL_FLASH_Unlock();
    }
		
		uint32_t  dataIndex;
		for(dataIndex=0;dataIndex<len;dataIndex++)
		{
				while(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,(uint32_t)(write_addr+dataIndex),*(uint8_t *)(writebuf+dataIndex))!=HAL_OK)delay_flash(100);
		}
		
		HAL_FLASH_Lock();//
}


void Flash_write(uint32_t write_addr,uint8_t* writebuf , uint32_t len,uint8_t Sector,uint8_t NSector)
{
    uint32_t offsetAddress;
	
	
    HAL_StatusTypeDef rc =HAL_ERROR;
	
	
    while(rc!=HAL_OK)
    {
      rc = HAL_FLASH_Unlock();
    }
    offsetAddress = write_addr;
    
    
    FLASH_EraseInitTypeDef f;
    
    f.TypeErase = FLASH_TYPEERASE_SECTORS;
    
 //   f.Banks = offsetAddress; 
    
    f.Sector = Sector;            // 
		
		f.NbSectors =NSector;          // 
		
		f.VoltageRange =FLASH_VOLTAGE_RANGE_2;
 
    uint32_t PageError = 0;
 
    
    HAL_FLASHEx_Erase(&f, &PageError);

 //   FLASH_PageErase(offsetAddress);//??????
			uint32_t  dataIndex;
		for(dataIndex=0;dataIndex<len;dataIndex++)
		{
				while(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,(uint32_t)(offsetAddress+dataIndex),*(uint8_t *)(writebuf+dataIndex))!=HAL_OK)delay_flash(100);
		}
		HAL_FLASH_Lock();//?????
}






/**
  * @}
  */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
