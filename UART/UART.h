/*
 * USART.h
 *
 * Created: 8/20/2014 10:28:33 AM
 *  Author: Mak
 */ 


#ifndef USART_H_
#define USART_H_

/*U Know these RIGHT?!*/
#define uchar		unsigned char
#define ulint       unsigned long

#define UART_0		0 //UART channel 0
#define UART_1		1 //UART channel 1


#define Baundrate	9600//baundrate of send and transmit
#ifndef F_CPU
#define F_CPU		8000000UL
#endif
#define MYUBBR		F_CPU/16UL/Baundrate-1 //Send this to init

#define ERROR		-1

//send to init to set parity mode
#define UART_DISABLE_PARITY 0 
#define UART_EVEN_PARITY 1
#define UART_ODD_PARITY 2

//send to init to set stop bit mode
#define UART_1BIT_STOP  0
#define UART_2BIT_STOP  1

/* INTERRUPS */
#define TX_COMPLETE_INT0 0 //if set(1) enables transmit complete interrupt for UART0
#define TX_COMPLETE_INT1 0 //if set(1) enables transmit complete interrupt for UART1
#define RX_COMPLETE_INT0 0 //if set(1) enables receive complete interrupt for UART0
#define RX_COMPLETE_INT1 0 //if set(1) enables receive complete interrupt for UART1
#define DATAREGISTER_EMPTY_INT0 0 //if set(1) enables data register empty interrupt for UART0
#define DATAREGISTER_EMPTY_INT1 0 //if set(1) enables data register empty interrupt for UART1

#endif /* USART_H_ */