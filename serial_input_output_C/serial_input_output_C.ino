/* Arduino Serial Input / output
* exemple in real C.
* In this file you can view how to get started with
* USART Init
* USART Received/Send data
* USART PutString
* How to use it : A led is connected to (arduino pin 13) PINB5
* Sending 'H' via serial will pull on the led
* Sending anything else will pull off the led
*/

// Include and CPU defines
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// BAUDRATE and prescaler definition
#define BAUDRATE 9600										//Baudrate to 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)	//Baudrate configuration

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

char String[]="Hello world!!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
char receiveByte;

// main function
int main(void){
	USART_init();        				//Call the USART initialization code
	USART_putstring(String);			//Sending the "hello world" String
        DDRB |= (1<<PINB5);				//Pin 5 of Port B pushed to OUTPUT
	while(1){        					//Infinite loop
	 receiveByte = USART_receive();		//Check received Byte
	 USART_send(receiveByte);			//Echo the received byte
	 if(receiveByte == 'H'){			//Checking if received byte is an 'H'
            PORTB |= (1<<PINB5);		//if so pull led on
          } else {
            PORTB &= ~(1<<PINB5);		//else pull it off
          }
}

return 0;
}

void USART_init(void){

 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){

 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
}

void USART_send( unsigned char data){

 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
}

void USART_putstring(char* StringPtr){

while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;}
}
