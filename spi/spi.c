/*
 * Spi.c
 *  Author: Mak
 *	This library is designed to help with spi interface of avr atmega series chip
 *  Tested with Atmega128 and Master mode.
 */ 

#include <avr/io.h>
#include "spi.h"

void spi_init(uchar master_slave,uchar data_order,uchar clock_polarity,uchar clock_phase,uchar speed){	
	
	if(master_slave){ //sets spi in master mode ss,mosi,sck output and miso as input
		SPI_DDR = (1<<SPI_SS)|(1<<SPI_SCK)|(1<<SPI_MOSI);    
		SPI_DDR &=~ (1<<SPI_MISO);
	}else{	//sets spi in slave mode ss,mosi,sck input and miso as output
		SPI_DDR &=~ ((1<<SPI_SS)|(1<<SPI_SCK)|(1<<SPI_MOSI));
		SPI_DDR |= (1<<SPI_MISO);
	}

		// Enables spi and sets other bits in SPCR as pasted to the function
		SPCR |= (1<<SPE)|(1<<master_slave)|(1<<data_order)|(1<<clock_polarity)|(1<<clock_phase)|(1<<SPI_INTERRUPT);
		/* SPCR default bit set is all 0s but because I use 0 shift as clear so have to clear 0's bit before 
		anything else(it seem better idea than bunch of else-ifs:)*/
		SPCR &=~ (1<<SPR0);
		
	if(master_slave){ //in master mode we have to set clock so we do
		if((speed==SPI_CLOCK_DIV8)||(speed==SPI_CLOCK_DIV16)||(speed==SPI_CLOCK_DIV128)){
			SPCR |=(1<<SPR0);
		}
		if((speed==SPI_CLOCK_DIV32)||(speed==SPI_CLOCK_DIV64)||(speed==SPI_CLOCK_DIV128)){
			SPCR |=(1<<SPR1);
		}
		if((speed==SPI_CLOCK_DIV2)||(speed==SPI_CLOCK_DIV8)||(speed==SPI_CLOCK_DIV32)){
			SPSR |=(1<<SPI2X);
		}
	}
	
}


uchar spi_read(void){
	
	return SPDR;
	
}
uchar spi_read_slave_mode(void){
	
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
	
}

uchar SPI_WriteRead(volatile uchar *slave_port,uchar slave_pin,uchar dataout)
{
	uchar datain;
	
	SPDR = dataout;// Start transmission (MOSI)
	while(!(SPSR & (1<<SPIF)));// Wait for transmission complete
	datain = SPDR;// Get return Value;
	
	*slave_port &=~ (1<<slave_pin);// Latch the Output using falling pulse to the RCK Pin
	_delay_us(1);// Hold pulse for 1 micro second
	*slave_port |= (1<<slave_pin);// Disable Latch
	//check for collision error
	if((SPSR & (1<<WCOL))){
		return ERROR;
	}
	// Return Serial In Value (MISO)
	return datain;
}

void spi_set_slave_select(volatile uchar *slave_ddr,volatile uchar *slave_port,uchar slave_pin){
	
	*slave_ddr |= (1<<slave_pin);
	*slave_port |= (1<<slave_pin);
	
}


uchar spi_status(void){	
	return SPSR;		
}

