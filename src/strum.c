#include "main.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"

#define BUTTONS_PORT GPIOB

static int old_reading;

static void adc_init() {
	//PB01, IN9, 12bit res, right align, 14MHz clk.

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= 0xC;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;

	while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
	ADC1->CR |= ADC_CR_ADEN;
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
}

static void button_init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(BUTTONS_PORT, &GPIO_InitStructure);
}

void strum_init() {
	adc_init();
	button_init();
}

static int adc_read() {
	ADC1->CHSELR = 0;
	ADC1->CHSELR |= 1 << 9; //IN9

	while(!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CR |= ADC_CR_ADSTART;
	while(!(ADC1->ISR & ADC_ISR_EOC));

	return ADC1->DR;
}

static int strum_hit() {
	int new = adc_read();

//	if((0x0223 < old_reading) && (old_reading < 0x02EB)) { //Idle Range
		if(((600 < new))) {
			return 1;
		}
//	}
	return 0;
}

static int buttons_read() {
	return (GPIO_ReadInputData(BUTTONS_PORT) >> 3) & 0x1f;
}

int check_if_note_hit(int song_position) {
	unsigned char notes = song[song_position];

	if (!notes) return 0;

if ((notes & buttons_read()) && strum_hit())
		return 1;
	else
		return -1;
}

void adc_read_first() {
	old_reading = adc_read();
}
