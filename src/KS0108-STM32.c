//-------------------------------------------------------------------------------------------------
// Universal KS0108 driver library
// STM32 MCU low-level driver
// (c) Rados³aw Kwiecieñ, radek@dxp.pl
//-------------------------------------------------------------------------------------------------
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"

#define KS0108_COMMAND_PORT  GPIOB
#define KS0108_EN    GPIO_Pin_10
#define KS0108_RS    GPIO_Pin_11
#define KS0108_RW    GPIO_Pin_12

#define KS0108_SWITCH_PORT GPIOC
#define KS0108_CS1   GPIO_Pin_4
#define KS0108_CS2   GPIO_Pin_5

#define KS0108_DATA_PORT GPIOA
#define KS0108_D0        5
// D0-D7 are in 5-12

#define DISPLAY_STATUS_BUSY	0x80

extern unsigned char screen_x;
extern unsigned char screen_y;

GPIO_InitTypeDef GPIO_InitStructure;

//-------------------------------------------------------------------------------------------------
// Delay function /for 8MHz/
//-------------------------------------------------------------------------------------------------
void GLCD_Delay(void)
{
  asm("nop");asm("nop");asm("nop");asm("nop");
}
//-------------------------------------------------------------------------------------------------
// Enable Controller (0-1)
//-------------------------------------------------------------------------------------------------
void GLCD_EnableController(unsigned char controller)
{
switch(controller){
	case 0 :
		GPIO_ResetBits(KS0108_SWITCH_PORT, KS0108_CS1);
		GPIO_SetBits(KS0108_SWITCH_PORT, KS0108_CS2);
		break;
	case 1 :
		GPIO_ResetBits(KS0108_SWITCH_PORT, KS0108_CS2);
		GPIO_SetBits(KS0108_SWITCH_PORT, KS0108_CS1);
		break;
	default: break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_DisableController(unsigned char controller)
{
switch(controller){
	case 0 :
		GPIO_ResetBits(KS0108_SWITCH_PORT, KS0108_CS2);
		break;
	case 1 :
		GPIO_ResetBits(KS0108_SWITCH_PORT, KS0108_CS1);
		break;
	default: break;
	}
}

//-------------------------------------------------------------------------------------------------
// Read Status byte from specified controller (0-2)
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadStatus(unsigned char controller)
{
unsigned char status;

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = 0xFF << KS0108_D0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(KS0108_DATA_PORT, &GPIO_InitStructure);

GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_RW);
GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_RS);
GLCD_EnableController(controller);
GLCD_Delay();
GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
status = ((GPIO_ReadInputData(KS0108_DATA_PORT) >> KS0108_D0) & 0xFF);
GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_DisableController(controller);
return status;
}
//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char commandToWrite, unsigned char controller)
{
while(GLCD_ReadStatus(controller) & DISPLAY_STATUS_BUSY);

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = (0xFF << KS0108_D0);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_Init(KS0108_DATA_PORT, &GPIO_InitStructure);

GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_RS | KS0108_RW);
GLCD_Delay();
GLCD_EnableController(controller);
GLCD_Delay();

GPIO_SetBits(KS0108_DATA_PORT, (commandToWrite << KS0108_D0));
commandToWrite ^= 0xFF;
GPIO_ResetBits(KS0108_DATA_PORT, (commandToWrite << KS0108_D0));
GLCD_Delay();
GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
GLCD_DisableController(controller);
}

//-------------------------------------------------------------------------------------------------
// Read data from current position
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
unsigned char tmp;
while(GLCD_ReadStatus(screen_x / 64) & DISPLAY_STATUS_BUSY);

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = 0xFF << KS0108_D0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(KS0108_DATA_PORT, &GPIO_InitStructure);

GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_RS | KS0108_RW);

GLCD_EnableController(screen_x / 64);
GLCD_Delay();
GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
tmp = ((GPIO_ReadInputData(KS0108_DATA_PORT) >> KS0108_D0) & 0xFF);
GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_DisableController(screen_x / 64);
screen_x++;
return tmp;
}
//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char dataToWrite)
{
while(GLCD_ReadStatus(screen_x / 64) & DISPLAY_STATUS_BUSY);
   
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = (0xFF << KS0108_D0);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_Init(KS0108_DATA_PORT, &GPIO_InitStructure);

GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_RW);
GLCD_Delay();
GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_RS);
GLCD_Delay();
GPIO_SetBits(KS0108_DATA_PORT, (dataToWrite << KS0108_D0));
dataToWrite ^= 0xFF;
GPIO_ResetBits(KS0108_DATA_PORT, (dataToWrite << KS0108_D0));
GLCD_Delay();
GLCD_EnableController(screen_x / 64);
GLCD_Delay();
GPIO_SetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
GPIO_ResetBits(KS0108_COMMAND_PORT, KS0108_EN);
GLCD_Delay();
GLCD_DisableController(screen_x / 64);
screen_x++;
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitializePorts(void)
{
// Currently using GPIO A, B and C
RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_All;
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

// Initialize Command Ports
GPIO_InitStructure.GPIO_Pin   =  KS0108_RS | KS0108_RW;
GPIO_Init(KS0108_COMMAND_PORT, &GPIO_InitStructure);
GPIO_Write(KS0108_COMMAND_PORT, KS0108_RS | KS0108_RW);

// Initialize Controller Switch Port
GPIO_InitStructure.GPIO_Pin   =  KS0108_CS1 | KS0108_CS2;
GPIO_Init(KS0108_SWITCH_PORT, &GPIO_InitStructure);
GPIO_Write(KS0108_COMMAND_PORT, KS0108_CS1 | KS0108_CS2);

// Initialize Data Pins
GPIO_InitStructure.GPIO_Pin   =  0xff << KS0108_D0;
GPIO_Init(KS0108_DATA_PORT, &GPIO_InitStructure);
GPIO_Write(KS0108_DATA_PORT, (0xFF << KS0108_D0));
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadByteFromROMMemory(char * ptr)
{
  return *(ptr);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
