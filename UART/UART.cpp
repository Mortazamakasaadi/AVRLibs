/*
 * USART.cpp
 *
 * Created: 8/20/2014 10:28:08 AM
 *  Author: Mak
 */ 

#include <avr/io.h>
#include "UART.h"
#include <stdlib.h>
#include <stdio.h>
class UART {
	
	private:
		
		/*sets interrupts of UART
			return: void
			parameter: void
		*/
		void set_intrupt(void){
			if(TX_COMPLETE_INT0){
				UCSR0B |=(1<<TXCIE0);
			}
			if(TX_COMPLETE_INT1){
				UCSR1B |=(1<<TXCIE1);
			}
			if(RX_COMPLETE_INT0){
				UCSR0B |= (1<<RXCIE0);
			}
			if(RX_COMPLETE_INT1){
				UCSR1B |= (1<<RXCIE1);
			}
			if(DATAREGISTER_EMPTY_INT0){
				UCSR0B |= (1<<UDRIE0);
			}
			if(DATAREGISTER_EMPTY_INT1){
				UCSR1B |= (1<<UDRIE1);
			}
		}
	public:
		
		/* UART init
			return:void
			parameter:
				channel: UART channel
				myubbr: UBBR register value
				txen: do you want enable transmit
				rxen: enable receive
				parity: set parity mode of data
				stop: 2bit stop or 1bit?!
		*/
		void init(uchar channel,ulint myubbr,uchar txen,uchar rxen,uchar parity,uchar stop){
			if(!channel){
				
				UBRR0H = ((uchar) (myubbr>>8));
				UBRR0L = ((uchar) (myubbr));
				UCSR0A &=~ (1<<FE0)|(1<<DOR0);
				if(txen==1){enable_tx(channel);}else{disable_tx(channel);}
				if(rxen==1){enable_tx(channel);}else{disable_tx(channel);}
				UCSR0B &=~ (1<<0);
				UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
				
				if(parity == UART_DISABLE_PARITY){
					UCSR0C &=~ (1<<UPM00)|(1<<UPM01);
				}else if(parity == UART_EVEN_PARITY){
					UCSR0C |=(1<<UPM01);
				}else if(parity == UART_ODD_PARITY){
					UCSR0C |=(1<<UPM00)|(1<<UPM01);
				}
				
				if(stop == UART_2BIT_STOP){
					UCSR0C |=(1<<USBS0);
				}
				
			}else{
				
				UBRR1H = ((uchar) (myubbr>>8));
				UBRR1L = ((uchar) (myubbr));
				UCSR1A &=~ (1<<FE1)|(1<<DOR1);
				
				if(txen==1){enable_tx(channel);}else{disable_tx(channel);}
				if(rxen==1){enable_tx(channel);}else{disable_tx(channel);}
					
				UCSR1B &=~ (1<<0);
				UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
				
				if(parity == UART_DISABLE_PARITY){
					UCSR1C &=~ (1<<UPM10)|(1<<UPM11);
					}else if(parity == UART_EVEN_PARITY){
					UCSR1C |=(1<<UPM11);
					}else if(parity == UART_ODD_PARITY){
					UCSR1C |=(1<<UPM10)|(1<<UPM11);
				}
				
				if(stop == UART_2BIT_STOP){
					UCSR1C |=(1<<USBS1);
				}
				
			}
			set_intrupt();
		}
		
		/*Default init for my projects
			return:void
			parameter:
				channel: UART channel
				myubbr: UBBR register value
		*/
		void init(uchar channel,ulint myubbr){
			if(!channel){
				UBRR0H = ((uchar) (myubbr>>8));
				UBRR0L = ((uchar) (myubbr));
				UCSR0A &=~ (1<<FE0)|(1<<DOR0);
				UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
				UCSR0B &=~ (1<<0);
				UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
				UCSR0C &=~ (1<<UPM00)|(1<<UPM01);
				UCSR0C |=(1<<USBS0);
				}else{
				UBRR1H = ((uchar) (myubbr>>8));
				UBRR1L = ((uchar) (myubbr));
				
				UCSR1A &=~ (1<<FE1)|(1<<DOR1);
				
				UCSR1B |= (1<<RXEN1)|(1<<TXEN1);
				UCSR1B &=~ (1<<0);
				
				UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10)|(1<<USBS1);
				UCSR1C &=~ (1<<UPM10)|(1<<UPM11);
			}
			set_intrupt();
		}
		
		/*Flushes receive buffer of UART
			return:flushed data
			parameter:
				channel: UART channel
		*/
		uchar flush(uchar channel){
			volatile uchar dummy;
			if(!channel){
				while(UCSR0A & (1<<RXC0)); dummy = UDR0;
			}else{
				while(UCSR1A & (1<<RXC1)); dummy = UDR1;
			}
			return dummy;
		}
	
		/*Sends char(8bit) over UART
			return:void
			parameter:
				channel: UART Channel
				data: 8bit tobe sent
		*/
		void trasmit(uchar channel,char data){
			if(!channel){
				while(!(UCSR0A&(1<<UDRE0)));
				UDR0 = data;
			}else{
				while(!(UCSR1A&(1<<UDRE1)));
				UDR1 = data;
			}
		}
		
		/*Sends int over UART
			return:void
			parameter:
				channel: UART Channel
				data: integer tobe sent
		*/
		void transmit(uchar channel,int data){
			char str[20];
			itoa(data,str,10);
			trasmit(channel,str);
		}
		
		/*Sends string over UART
			return:void
			parameter:
				channel: UART Channel
				str: char array tobe sent
		*/
		void trasmit(uchar channel,const char str[]){
			char j;
			for(int i=0;;i++){
				j=str[i];
				if(j == '\0'){
					break;
				}
				trasmit(channel,str[i]);
			}
		}
		
		/*Receives char 
			return:received char
			parameter:UART channel
		*/
		uchar receive(uchar channel){
			if(!channel){
				while(!(UCSR0A & (1<<RXC0)));
				
				return UDR0;
			}else{
				while(!(UCSR1A & (1<<RXC1)));
				
				return UDR1;
			}
			
		}
		
		/*receive char arrays
			return:void (change passed array directly)
			parameter: 
				channel: UART channel
				str: pass this as buffer for received string
				size: size of data you want to receive
		*/
		void receive(uchar channel,char* str,int size){
			 int i=0;
			 while (i < size-1) {            
				 unsigned char c;
				 c = receive(channel);
				 if (c == '\0') break;          
				 str[i] = c;                     
				 i++;
			 }
			 str[i] = 0;
		}
		
		
		/*Disables receive
			return: void
			parameter: UART Channel
		*/
		void disable_rx(uchar channel){
			if(!channel){
				UCSR0B &=~ (1<<RXEN0);
			}else{
				UCSR1B &=~ (1<<RXEN1);
			}
			flush(channel);
		}
		
		/*Disables transmit
			return: void
			parameter: UART Channel
		*/
		void disable_tx(uchar channel){
			if(!channel){
					UCSR0B &=~ (1<<TXEN0);
				}else{
					UCSR1B &=~ (1<<TXEN1);
			}
		}
		
		/*Enables receive
			return: void
			parameter: UART Channel
		*/
		void enable_rx(uchar channel){
			if(!channel){
					UCSR0B |= (1<<RXEN0);
				}else{
					UCSR1B |= (1<<RXEN1);
			}
		}
		
		/*Enables transmit
			return: void
			parameter: UART Channel
		*/
		void enable_tx(uchar channel){
			if(!channel){
					UCSR0B |= (1<<TXEN0);
				}else{
					UCSR1B |= (1<<TXEN1);
			}
		}
		
		/*Enables double speed mode
			return: void
			parameter: UART Channel
		*/
		void double_speed(uchar channel){
			if (!channel){
				UCSR0A |= (1<<U2X0);
			}else{
				UCSR1A |= (1<<U2X1);
			}
		}
		
		/*Disables double speed mode
			return: void
			parameter: UART Channel
		*/
		void disable_double_speed(uchar channel){
			if (!channel){
				UCSR0A &=~ (1<<U2X0);
				}else{
				UCSR1A &=~ (1<<U2X1);
			}
		}
	};