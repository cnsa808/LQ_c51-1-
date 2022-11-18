#include<STC15F2K60S2.H>
#include "intrins.h"
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1
unsigned char date;
//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void Delay2ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}
//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}
//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//应答位控制
void IIC_Ack(bit ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

unsigned char adc_read(unsigned char addr0)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(addr0);
	IIC_WaitAck();
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	date=IIC_RecByte();
	IIC_WaitAck();
	IIC_Ack(0);
	IIC_Stop();
	return date;		
}

void dac_writer(unsigned char addr2)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(addr2);
	IIC_WaitAck();
	IIC_Ack(0);
	IIC_Stop();
}

unsigned char eeprom_read(unsigned char addr1)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr1);
	IIC_WaitAck();
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	date=IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return date;
	Delay2ms();		
}
void eeprom_writer(unsigned char addr2,date3)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr2);
	IIC_WaitAck();
	IIC_SendByte(date3);
	IIC_WaitAck();
	IIC_Stop();
	Delay2ms();	
}