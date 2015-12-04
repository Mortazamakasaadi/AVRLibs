/*
 * Spi.h
 *
 * Created: 8/10/2014 7:51:40 PM
 *  Author: Mak
 */ 


#ifndef SPI_H_
#define SPI_H_

#ifndef F_CPU
/* prevent compiler error by supplying a default */
# warning "F_CPU not defined"
# define F_CPU 8000000UL
#endif

#define	uchar						unsigned char					
#define ERROR						-1

#define SPI_DDR						DDRB
#define SPI_PORT					PORTB
#define SPI_SS						PB0
#define SPI_SCK						PB1
#define SPI_MOSI					PB2
#define SPI_MISO					PB3


#define _SPI_INTERRUPT				0	//if set(1) enables spi interrupt
#define SPI_DATA_ORDER_LSB_FIRST	0	//0 for LSB first data transfer
#define SPI_DATA_ORDER_MSB_FIRST	5	//for MSB first data transfer
#define SPI_MASTER					4	//sets spi for master mode(mcu is master)
#define SPI_SLAVE					0	//clears spi for slave mode(mcu will become slave)
#define SPI_HIGH_IDLE				3	//sets spi clock polarity(spi works on low clock cycles)
#define SPI_LOW_IDLE				0   //clears spi clock polarity(spi works on high clock cycles)
#define SPI_SAMPLE_ON_LEADING_EDGE  0	//data samples on leading edge of sck
#define SPI_SAMPLE_ON_TRAILING_EDGE 2   //data samples on trailing edge of sck

#if _SPI_INTERRUPT==1
#define SPI_INTERRUPT	7
#else 
#define SPI_INTERRUPT	0
#endif

//define the frequency of spi's clock 
//		spi clock = mcu clock / div"number"

#define SPI_CLOCK_DIV2				1
#define SPI_CLOCK_DIV4				2
#define SPI_CLOCK_DIV8				3
#define SPI_CLOCK_DIV16				4
#define SPI_CLOCK_DIV32				5
#define SPI_CLOCK_DIV64				6
#define SPI_CLOCK_DIV128			7

/* 
inits the spi 
	parameters:
		master_slave: sets spi in either master or slave mode
		data_order: sets msb first or lsb first mode
		clock_polarity: sets idle on high or idle on low sck
		clock_phase: sets sampling on trailing edge or sampling on leading edge
		speed: sets the transmission speed division factor 
	return: void
*/
extern void spi_init(uchar master_slave,uchar data_order,uchar clock_polarity,uchar clock_phase,uchar speed);

/*
 writes data to SPDR register and sends it to slave
	parameters:
		slave_port: salve's select line's port
		slave_pin: salve's select line's pin
		dataout: data tobe send (8bit)
	return: SPDR (returned data from slave)
*/

extern uchar SPI_WriteRead(volatile uchar *slave_port,uchar slave_pin,uchar dataout);

/* 
read SPDR register
	parameters: void
	return: SPDR register's value
*/

extern uchar spi_read(void);

/*
 read data in slave mode
	parameters: void
	return: SPDR register's value
*/

extern uchar spi_read_slave_mode(void);

/* 
sets slave's select line 
	parameters: 
		slave_ddr: slave's data direction, exp: &DDRA
		slave_port: port which slave's select line connected, exp: &PORTA
		slave_pin: pin which slave select line is connected, exp: PA0
	return: void
*/

extern void spi_set_slave_select(volatile uchar *slave_ddr,volatile uchar *slave_port,uchar slave_pin);

/* 
read SPSR register (spi status register)
	parameters: void
	return: SPSR register's value
*/

extern uchar spi_status(void);

#endif /* SPI_H_ */