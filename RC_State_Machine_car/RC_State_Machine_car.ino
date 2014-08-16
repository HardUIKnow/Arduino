/* define internal for the MEGA as 1.1V (as as for the 328)  */
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define INTERNAL INTERNAL1V1
#endif

/* define motor structure                                  */
typedef struct { int dirA; int dirB; int speedA; int speedB;} Motor;    
volatile Motor Mot[3] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}}; /*  3 motor structure with "advance/backward" "turn left/right" "stop" */
const int M1 = 12;
const int M2 = 13;
const int M1Speed = 3;
const int M2Speed = 11;

void setup(){
  /* Initialize serial */
  Serial.begin(115200);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M1Speed, OUTPUT);
  pinMode(M2Speed, OUTPUT);
}

void loop(){

  /* VAriable declaration and initialisation */
  static int s= -1;             /* state */
  static int mot = 0;           /* generic motor state */
  int val =0;
  int dir =0 ;
  if (Serial.available() >0) {

    /* whatever is available from the serial is read here    */
    val = Serial.read();
    switch (s) {
      
      /* s = -1 means nothing */
      case -1:
      
      /* calculate next state                                */
      if (val>47 && val<90) {
	  /* the first received value indicates the mode       
           49 is ascii for 1, ... 90 is ascii for Z          
           s=0 is motor stop;
           s=10 is advance/backward;  s=20 is turn left/right;
                                                             */
        s=10*(val-48);
      }
      
      if ((s>20 && s<210) || (s>250 && s!=340 && s!=400)) {
        s=-1;
      }

      /* the break statements gets out of the switch-case, so
      /* we go back and wait for new serial data             */
      break; /* s=-1 (initial state) taken care of           */
      
      /* s=0 or 1 means motor stop */
      case 10:
      if (val>47 && val<51){
        /* motor stop */
        mot=val-48;
        Serial.println(mot);
        s=11; /* next we need the first motor direction */
      }
      else {
        s = -1;
      }
      break;
      
      case 11:
      if(val>47 && val<50){
        dir = val-48; /* get direction 0 forward 1 backward */
        if(dir == 1){
          Mot[mot].dirA = HIGH; /* motor direction */
        } else {
          Mot[mot].dirA = LOW;
        }
        s=12; /*next direction setup */
        Serial.println(Mot[mot].dirA);
      }
      else {
        s= -1;
      }
      break;
      
      case 12:
        if(val>47 && val<50){
        dir = val-48; /* get direction 0 forward 1 backward */
        if(dir == 1){
          Mot[mot].dirB = HIGH; /* motor direction */
        } else {
          Mot[mot].dirB = LOW;
        }
        s=13; /*next speedA setup */
      }
      else {
        s= -1;
      }
      break;
      
      case 13:
        Mot[mot].speedA = val;
        s=14;
      break;
      
      case 14:
        Mot[mot].speedB = val;
        s=15;
      break;
      
      case 15:
      Serial.println(Mot[mot].dirA);	//debug
      Serial.println(Mot[mot].dirB);	//debug
      Serial.println(Mot[mot].speedA);	//debug
      Serial.println(Mot[mot].speedB);	//debug
      digitalWrite(M1, Mot[mot].dirA);	
      digitalWrite(M2, Mot[mot].dirB);	
      analogWrite(M1Speed, Mot[mot].speedA);
      analogWrite(M2Speed, Mot[mot].speedB);
      
      s = -1;
      break;
      
      default:
      
      s= -1;
      
    } /* end switch state */
  }   /* end serial request */
}     /* end loop */

      
    
          

  

