

#include <Wire.h>

#define I2C_HIGHDRIVER4_ADRESS  (0x79)  //Default adress for Highdriver4 0x79 on Multiboard2

#define I2C_DEVICEID       0x00         //Register adresses
#define I2C_POWERMODE      0x01
#define I2C_FREQUENCY      0x02
#define I2C_SHAPE          0x03
#define I2C_BOOST          0x04
#define I2C_PVOLTAGE       0x06
#define I2C_P1VOLTAGE      0x06
#define I2C_P2VOLTAGE      0x07
#define I2C_P3VOLTAGE      0x08
#define I2C_P4VOLTAGE      0x09
#define I2C_UPDATEVOLTAGE  0x0A
#define I2C_AUDIO          0x05
int pre =0;
boolean bPumpState[4] = {false,false,false,false};
uint8_t nPumpVoltageByte[4] = {0x00,0x00,0x00,0x00};
uint8_t nFrequencyByte = 0x40;
const int fertilized = 1;
const int unfertilized = 2;
int LED_BUILTIN=2;
bool Highdriver_check(void) {
  Wire.beginTransmission(I2C_HIGHDRIVER4_ADRESS);
  Wire.write(I2C_DEVICEID);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(I2C_HIGHDRIVER4_ADRESS,1);
  uint8_t data = Wire.read();
  return ((data&0xF0)==0xB0);
}

void Highdriver_init(void) {
  Wire.beginTransmission(I2C_HIGHDRIVER4_ADRESS);
  Wire.write(I2C_POWERMODE);                                          // Start Register 0x01
  Wire.write(0x01);                                                   // Register 0x01 = 0x01 (enable)
  Wire.write(nFrequencyByte);                                         // Register 0x02 = 0x40 (100Hz)
  Wire.write(0x00);                                                   // Register 0x03 = 0x00 (sine wave)
  Wire.write(0x00);                                                   // Register 0x04 = 0x00 (800KHz)
  Wire.write(0x00);                                                   // Register 0x05 = 0x00 (audio off)
  Wire.write(0x00);                                                   // Register 0x06 = Amplitude1
  Wire.write(0x00);                                                   // Register 0x07 = Amplitude2
  Wire.write(0x00);                                                   // Register 0x08 = Amplitude3
  Wire.write(0x00);                                                   // Register 0x09 = Amplitude4
  Wire.write(0x01);                                                   // Register 0x0A = 0x01 (update)
  Wire.endTransmission();
  bPumpState[3] = false; 
  nPumpVoltageByte[3] = 0x1F;
}

void Highdriver4_setvoltage(uint8_t _pump, uint8_t _voltage) {           //Highdriver4
  float temp = _voltage; temp*=31.0f; temp/=250.0f;   //250Vpp = 0x1F
  if (_pump>=1 && _pump<=4) nPumpVoltageByte[_pump-1]=constrain(temp,0,31);
  Wire.beginTransmission(I2C_HIGHDRIVER4_ADRESS);
  Wire.write(I2C_PVOLTAGE);  
  Wire.write((bPumpState[0] ? nPumpVoltageByte[0] : 0));
  Wire.write((bPumpState[1] ? nPumpVoltageByte[1] : 0));
  Wire.write((bPumpState[2] ? nPumpVoltageByte[2] : 0));
  Wire.write((bPumpState[3] ? nPumpVoltageByte[3] : 0));
  Wire.write(0x01);                                                     // update new driver Entries
  Wire.endTransmission(); 
}

void Highdriver4_setvoltage(void) {                                      //  Update amplitude at Highdriver4
  Wire.beginTransmission(I2C_HIGHDRIVER4_ADRESS);
  Wire.write(I2C_PVOLTAGE);  
  Wire.write((bPumpState[0] ? nPumpVoltageByte[0] : 0));
  Wire.write((bPumpState[1] ? nPumpVoltageByte[1] : 0));
  Wire.write((bPumpState[2] ? nPumpVoltageByte[2] : 0));
  Wire.write((bPumpState[3] ? nPumpVoltageByte[3] : 0));
  Wire.write(0x01);                                                     // update new driver Entries
  Wire.endTransmission(); 
}

void Highdriver_setfrequency(uint16_t _frequency) {
  if (_frequency>=800) {
    nFrequencyByte=0xFF;
  } else if (_frequency>=400) {                                     // Range 400-800 Hz
    _frequency-=400;
    _frequency*=64;
    _frequency/=400;
    nFrequencyByte = _frequency|0xC0;
  } else if (_frequency>=200) {                                     // Range 200-400 Hz
    _frequency-=200;
    _frequency*=64;
    _frequency/=200;
    nFrequencyByte = _frequency|0x80;
  } else if (_frequency>=100) {                                     // Range 100-200 Hz
    _frequency-=100;
    _frequency*=64;
    _frequency/=100;
    nFrequencyByte = _frequency|0x40;
  } else if (_frequency>=50) {                                      // Range 50-100 Hz
    _frequency-=50;
    _frequency*=64;
    _frequency/=50;
    nFrequencyByte = _frequency|0x00;
  } else {                                                         // Out of Frequncy Boundery
    nFrequencyByte=0x00;
  }
  Wire.beginTransmission(I2C_HIGHDRIVER4_ADRESS);
  Wire.write(I2C_FREQUENCY);
  Wire.write(nFrequencyByte);
  Wire.endTransmission();  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  Highdriver_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  bPumpState[0] = true;
  bPumpState[1] = true;
  bPumpState[2] = true;

  

  if (Serial.available() > 0) {
    // Read in request
    int inByte = Serial.read();

    // Take appropriate action
      if (inByte==1) 
      {
        digitalWrite(LED_BUILTIN,HIGH);
        Highdriver4_setvoltage(1,100);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(2,250);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(3,0);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver_setfrequency(200);   //set frequency to 100 Hz
        delay(1000); 
        Highdriver4_setvoltage(1,100);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(2,200);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(3,0);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver_setfrequency(200);   //set frequency to 100 Hz
        delay(2000); 
        digitalWrite(LED_BUILTIN,LOW);


      }
      if (inByte==0) 
      {
      
        digitalWrite(LED_BUILTIN,HIGH);

        Highdriver4_setvoltage(1,100);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(2,0);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(3,250);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver_setfrequency(100);   //set frequency to 100 Hz
        delay(1000);
        Highdriver4_setvoltage(1,100);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(2,0);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver4_setvoltage(3,250);     //Turn pump on and set pump amplitude to 250Vpp
        Highdriver_setfrequency(100);   //set frequency to 100 Hz
        delay(2000);
        digitalWrite(LED_BUILTIN,LOW);

        
      
              }
  }


if (pre ==0)
{
   Highdriver4_setvoltage(1,250);     //Turn pump on and set pump amplitude to 250Vpp
  Highdriver4_setvoltage(2,250);       //Turn pump on and set pump amplitude to 250Vpp
  Highdriver4_setvoltage(3,2500);  
  Highdriver_setfrequency(100);      //set frequency to 100 Hz
  delay(2000);
  pre =1 ;
}

if (pre ==1)
{
  Highdriver4_setvoltage(1,80);     //Turn pump on and set pump amplitude to 250Vpp
  Highdriver4_setvoltage(2,100);       //Turn pump on and set pump amplitude to 250Vpp
  Highdriver4_setvoltage(3,100);  
  Highdriver_setfrequency(100);      //set frequency to 100 Hz
  
}
 
  

}





 
