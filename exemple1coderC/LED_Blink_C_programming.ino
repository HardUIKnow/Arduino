/* this an exemple of arduino programming
using real microcontroler programming
with C instruction int the typical
LED blinking exemple...
Added by HardUIKnow
*/

// include 
#include <avr/delay.h>

// main function
int main(void){

// Port B Pin 5 aka arduino pin 13 directed to OUTPUT
 DDRB |= (1 << PINB5);
 
 // main loop
 while(1)
 {
   // Pushing pin to logic state 1
     PORTB = (1 << PINB5);
   // delay 1s or 1000 ms 
	_delay_ms(1000);
   // Pushing pin to logic state 0
     PORTB = (0 << PINB5);
   //delay 1s
   _delay_ms(1000);
 }
return 0; 
// end main
}
