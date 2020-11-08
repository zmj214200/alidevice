#include "trans_recieve_buff_control.h"
#include "ringbuffer.h"
#include "main.h"
RingBuffer  m_Modbus_Master_RX_RingBuff;
uint8_t     m_Modbus_Master_RX_Buff[200];

extern UART_HandleTypeDef huart6;
/**
  * @brief  ��ʼ���жϽ��յĵ�ringbuffer���ζ�������,�жϽ��յ��ֽڶ���m_Modbus_Master_RX_RingBuff�ýṹ��ָ����й���
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_RB_Initialize(void)
{
	/*��ʼ��ringbuffer��ص�����*/
	rbInitialize(&m_Modbus_Master_RX_RingBuff, m_Modbus_Master_RX_Buff, sizeof(m_Modbus_Master_RX_Buff));
	return 1 ;
}


/**
  * @brief  ������ζ���
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Flush(void)
{
  rbClear(&m_Modbus_Master_RX_RingBuff);
	return 0;
}
/**
  * @brief  �ж�GPS��ringbuffer�����Ƿ�����δ�������ֽ�
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Available(void)
{
	/*������ݰ�buffer��������ˣ������㣬���¼���*/
		if(m_Modbus_Master_RX_RingBuff.flagOverflow==1)
		{
			rbClear(&m_Modbus_Master_RX_RingBuff);
		}
	return !rbIsEmpty(&m_Modbus_Master_RX_RingBuff);
}

/****************************************************************************************************/
/*��������Ӳ���ӿڲ�������Ĳ��֣����ݲ�ͬ�������Ĵ�����ʽ������ֲ*/
/**
  * @brief  ��ȡ���ռĴ����������ֵ
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_GetByte(uint8_t  *getbyte)
{
  if(HAL_UART_Receive (&huart6 ,(uint8_t *)getbyte,1,0x05) != HAL_OK )
  {
		return HAL_ERROR;
	}
	else
	{
	  return HAL_OK;
	}
}

/**
  * @brief  �жϴ����������ڴ��ڽ����ж��е��ã����Ĵ�������ֵѹ�˻�����
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Handler(void)
{
	 uint8_t byte;
	//��ȡ�Ĵ�����������ݣ����ҽ�����ѹ�뻷�ζ���
   if(Modbus_Master_GetByte(&byte)==HAL_OK)
	 {
	   rbPush(&m_Modbus_Master_RX_RingBuff, (uint8_t)(byte & (uint8_t)0xFFU));
	 }
	 return 0;
}

/**
  * @brief  ����������������
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Read(void)
{
	uint8_t cur =0xff;
	if( !rbIsEmpty(&m_Modbus_Master_RX_RingBuff))
	{
		  cur = rbPop(&m_Modbus_Master_RX_RingBuff);
	}
	return cur;
}

/**
  * @brief  �����ݰ����ͳ�ȥ
  * @param
  * @note
  * @retval void
  * @author xiaodaqi#define RS485_EN0_Pin GPIO_PIN_8
#define RS485_EN0_GPIO_Port GPIOC
  */
uint8_t Modbus_Master_Write(uint8_t *buf,uint8_t length)
{
 if(HAL_UART_Transmit(&huart6 ,(uint8_t *)buf,length,0xff))
 {
   return HAL_ERROR;
 }
 	else
	{
	  return HAL_OK;
	}
}

/**
  * @brief  1ms���ڵĶ�ʱ��
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint32_t Modbus_Master_Millis(void)
{
  return HAL_GetTick();
}