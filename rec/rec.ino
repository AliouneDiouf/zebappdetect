const int LED_ON = 1;
const int LED_OFF = 2;



int state = 0; 
void setup() {

  Serial.begin(9600);
  Serial.setTimeout(1);

  pinMode(2, OUTPUT);

}



void loop() 
{
      
    if (state == 0)
    {
        digitalWrite(2, HIGH);
        delay(1000);
        digitalWrite(2, LOW);
        delay(1000);
    }
   
            
    if (Serial.available()> 0) 
    {
      //int inByte = Serial.read();
       digitalWrite(2, HIGH);
        delay(4000); 
    // Read in request
    // int inByte = Serial.read();
    // delay(10);
    //if (inByte == 1)
    // {
     //  digitalWrite(2, HIGH);
     //  delay(4000);   
    //}

    //  else 
    //{
     // digitalWrite(2, LOW);
      //delay(4000); 
    //}

  }
 
}   
 
  
  
