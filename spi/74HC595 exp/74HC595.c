/*
 * GccApplication3.c
 *
 * Created: 8/11/2014 10:06:22 PM
 *  Author: Mak
 */ 

# define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "spi.c"

#define right_to_left	0
#define left_to_right	1

unsigned char move[]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void blink(void);
void right_left(unsigned char m[],uint8_t dir);
void one_left(void);
void oneon_onoff(void);

int main(void)
{
	spi_init(SPI_MASTER,SPI_DATA_ORDER_LSB_FIRST,SPI_LOW_IDLE,SPI_SAMPLE_ON_LEADING_EDGE,SPI_CLOCK_DIV128);
	spi_set_slave_select(&DDRA,&PORTA,PA0);
	spi_set_slave_select(&DDRA,&PORTA,PA1);
	
    while(1){
		 blink();
		 right_left(move,right_to_left);
		 right_left(move,left_to_right);
		 blink();
		 right_left(move,left_to_right);
		 right_left(move,right_to_left);
		 blink();
		 oneon_onoff();
		 }

}

void blink(void){
	for(uint8_t i=0;i<2;i++){
		      _delay_ms(75);
		      SPI_WriteRead(&PORTA,PA1,0xff);
		      SPI_WriteRead(&PORTA,PA0,0xff);
		      _delay_ms(100);
		      SPI_WriteRead(&PORTA,PA1,0x00);
		      SPI_WriteRead(&PORTA,PA0,0x00);
			  }
}

void right_left(unsigned char m[],uint8_t dir){
	if(dir==right_to_left){
	for (uint8_t i=1;i<9;i++){
		SPI_WriteRead(&PORTA,PA0,m[i]);
		_delay_ms(50);
	}
	SPI_WriteRead(&PORTA,PA0,0x00);
	for (uint8_t i=1;i<9;i++){
		SPI_WriteRead(&PORTA,PA1,m[i]);
		_delay_ms(50);
	}
	SPI_WriteRead(&PORTA,PA1,0x00);
	}
	else{
		for (uint8_t i=8;i>=1;i--){
			SPI_WriteRead(&PORTA,PA1,m[i]);
			_delay_ms(50);
		}
		SPI_WriteRead(&PORTA,PA1,0x00);
		for (uint8_t i=8;i>=1;i--){
			SPI_WriteRead(&PORTA,PA0,m[i]);
			_delay_ms(50);
		}
		SPI_WriteRead(&PORTA,PA0,0x00);
	}
}

void oneon_onoff(void){
	for (char i=16;i>=1;i--){
		SPI_WriteRead(&PORTA,PA0,0x55);
		_delay_ms(50);
		SPI_WriteRead(&PORTA,PA1,0x55);
		_delay_ms(50);
		SPI_WriteRead(&PORTA,PA0,0xaa);
		_delay_ms(50);
		SPI_WriteRead(&PORTA,PA1,0xaa);
		_delay_ms(50);
	}
}

