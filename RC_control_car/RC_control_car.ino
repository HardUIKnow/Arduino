/*
**	Xbee RC car control 
** This sketch demonstrate the use of a full remote controlled
** RC car using xbee module
** an IHM is provided to get data from the arduino
** including :
** 				car direction (N, E, O, S, etc...)
**				car speed 
**				car batterie
**				Picture from the platform camera
** Todo :
**				WIFI version of the main program 
**				Signal strengh for the WIFI version
**				better screenshot function (need a fix cause picture is not always lunched)
**				code improvement (some function or libraries not fuly used and can save space)
*/

#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Adafruit_VC0706.h>
#include <SD.h>

// comment out this line if using Arduino V23 or earlier
#include <SoftwareSerial.h> 
#include <Wire.h>

float angles[3]; // yaw pitch roll

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();
#define DEVICE (0x53) // Device address as specified in data sheet
#define chipSelect 4

// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(2, 3);
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(2, 3);
#endif

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Using hardware serial on Mega: camera TX conn. to RX1,
// camera RX to TX1, no SoftwareSerial object is required:
//Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);
uint16_t jpglen = 0;
boolean getImage = false;
byte _buff[6];

char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1


//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control
long lastTime = 0;
int accel=0;
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
  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT); 
  Serial.begin(9600);      //Set Baud Rate
  Serial.println("Run keyboard control");
  Wire.begin();
  
  delay(5);
  sixDOF.init(); //begin the IMU
    //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  DATA_FORMAT = 0b00001000;  // D3 to 1 Full resolution mode
  //writeTo(0x2C, 0x0A); // Data rate 100hz sampling mode
  writeTo(DATA_FORMAT, 0x03); // +/- 16G mode
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeTo(POWER_CTL, 0x08);
  
  delay(5);
  
    // When using hardware SPI, the SS pin MUST be set to an
  // output (even if not connected or used).  If left as a
  // floating input w/SPI on, this can cause lockuppage.
#if !defined(SOFTWARE_SPI)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if(chipSelect != 53) pinMode(53, OUTPUT); // SS on Mega
#else
  if(chipSelect != 10) pinMode(10, OUTPUT); // SS on Uno, etc.
#endif
#endif

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
}
void loop(void)
{
if(getImage != true){    
  sixDOF.getEuler(angles);
  Serial.print("CAN(");
  readAccel(); // read the x/y/z tilt

  Serial.print(angles[0]); Serial.print(',');
  Serial.print(angles[1]); Serial.print(',');
  Serial.print(angles[2]); Serial.print(',');
  Serial.print(readVcc());
  Serial.println(")");
}
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
        move (&speed1, &speed2, &motor1, &motor2);
        break;      
      case 'd'://Turn Right
        speed1 = 255; speed2 = 255; motor1 = HIGH; motor2 = LOW;
        move (&speed1, &speed2, &motor1, &motor2);
        break;
      case 'x':
        speed1 = 0; speed2 = 0; motor1 = HIGH; motor2 = HIGH;
        move (&speed1, &speed2, &motor1, &motor2);
        break;
      case 'l':
        getImage = true;
        Serial.flush();
        delay(25);
        Serial.println("");
        photo(); 
        Serial.println(jpglen, DEC);
        break;
      case 'p':
        sendPicture();
        getImage =false;
        break; 
      }
    }
    else {
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

void readAccel() {
  uint8_t howManyBytesToRead = 6;
  readFrom( DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  int x = (((int)_buff[1]) << 8) | _buff[0];
  int y = (((int)_buff[3]) << 8) | _buff[2];
  int z = (((int)_buff[5]) << 8) | _buff[4];
//  Serial.print("x: ");
//  Serial.print( x );
//  Serial.print(" y: ");
  Serial.print( (abs(x) * 3600) / 1000 );
  Serial.print(',');
  Serial.print( (abs(y) * 3600) / 1000 );
  Serial.print(',');
  Serial.print( (abs(z) * 3600) / 1000 );
  Serial.print(',');
  
//  Serial.print(" z: ");
//  Serial.println( z );
}

void writeTo(byte address, byte val) {
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // send register address
  Wire.write(val); // send value to write
  Wire.endTransmission(); // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void readFrom(byte address, int num, byte _buff[]) {
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // sends address to read from
  Wire.endTransmission(); // end transmission

  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.requestFrom(DEVICE, num); // request 6 bytes from device

  int i = 0;
  while(Wire.available()) // device may send less than requested (abnormal)
  {
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); // end transmission
}

void photo(){
  // Try to locate the camera
  if (cam.begin()) {
  //  Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }
  // Print out the camera version information (optional)
  char *reply = cam.getVersion();
  if (reply == 0) {
    Serial.print("Failed to get version");
  } 

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
  //cam.setImageSize(VC0706_640x480);        // biggest
  cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small

  // You can read the size back from the camera (optional, but maybe useful?)
  uint8_t imgsize = cam.getImageSize();
//  Serial.print("Image size: ");
//  if (imgsize == VC0706_640x480) Serial.println("640x480");
//  if (imgsize == VC0706_320x240) Serial.println("320x240");
  if (imgsize == VC0706_160x120) Serial.println("160x120");

//  Serial.println("Snap in 3 secs...");
//  delay(3000);

  if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
 // else 
 //   Serial.println("Picture taken!");
  
  
   jpglen = cam.frameLength();
 
}


void sendPicture() {

  // Get the size of the image (frame) taken  
 // Serial.print("Storing ");
//  Serial.print(jpglen, DEC);
//  Serial.print(" byte image.");

  int32_t time = millis();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    Serial.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
     // Serial.print('.');
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 // long internal11Ref = 1.1 * 5.0 / result;
 // long scale_constant = internal11Ref * 1023 * 1000;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

