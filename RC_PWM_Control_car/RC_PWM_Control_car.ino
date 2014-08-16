
//Standard PWM DC control
const int E1 = 3;     //M1 Speed Control
const int E2 = 11;     //M2 Speed Control
const int M1 = 12;    //M1 Direction Control
const int M2 = 13;    //M1 Direction Control
 uint8_t speed1 = 0;
 uint8_t speed2 = 0;
 uint8_t motor1 = LOW;
 uint8_t motor2 = LOW;
///For previous Romeo, please use these pins.
//int E1 = 6;     //M1 Speed Control
//int E2 = 9;     //M2 Speed Control
//int M1 = 7;    //M1 Direction Control
//int M2 = 8;    //M1 Direction Control

void setup(void)
{
  // 31khz PWM frequency
  TCCR2B = TCCR2B & 0b11111000 | 0x01; //Timer 2 (PWM pins 3 & 11)
  // connect motor pin
  pinMode(E1, OUTPUT); 
  pinMode(E2, OUTPUT); 
  pinMode(M1, OUTPUT); 
  pinMode(M2, OUTPUT); 

  Serial.begin(9600);      //Set Baud Rate

}
void loop(void)
{
  
  if(Serial.available()){
    uint8_t val = Serial.read();
    
    if(val != -1)
    {
      switch(val)
      {
      case 'z'://Move Forward
        speed1 = 255; speed2 = 255; motor1 = HIGH; motor2 = HIGH;
        move (&speed1, &speed2, &motor1, &motor2);   //move forward in max speed
        break;
      case 's'://Move Backward
        speed1 = 255; speed2 = 255; motor1 = LOW; motor2 = LOW;
        move (&speed1, &speed2, &motor1, &motor2);   //move back in max speed
        break;
      case 'q'://Turn Left
        speed1 = 255; speed2 = 255; motor1 = LOW; motor2 = HIGH;
        move (&speed1, &speed2, &motor1, &motor2);   //turn left in max speed
        break;      
      case 'd'://Turn Right
        speed1 = 255; speed2 = 255; motor1 = HIGH; motor2 = LOW;
        move (&speed1, &speed2, &motor1, &motor2);   //turn right in max speed
        break;
      case 'x': // Stop
        speed1 = 0; speed2 = 0; motor1 = LOW; motor2 = LOW;
        move (&speed1, &speed2, &motor1, &motor2);   //stop with motor speed to 0 and direction set to backward
        break;
      
      }
    }
    else { // default
        speed1 = 0; speed2 = 0; motor1 = HIGH; motor2 = HIGH;
        move (&speed1, &speed2, &motor1, &motor2);
    } 
  }
    delay(60);

}

void move(uint8_t *movespeed1,uint8_t *movespeed2, uint8_t *movemotor1, uint8_t *movemotor2)          //Move forward
{
  analogWrite (E1,*movespeed1);      //PWM Speed Control
  digitalWrite(M1,*movemotor1);   
  analogWrite (E2,*movespeed2);   
  digitalWrite(M2,*movemotor2);
} 

