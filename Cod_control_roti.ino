#include <IRremote.h>

int ena = 6;
int in1 = 5;
int in2 = 9;
int in3 = 10;
int in4 = 8;
const long  irPin =  7;


IRrecv irrecv(irPin);

 
void setup() {
 // pinMode(10,OUTPUT); //Sets the motors as an Output
 // pinMode(11,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ena,OUTPUT);
  Serial.begin(9600); //Starts the serial communication
  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);
}
 
void loop() { 
   if ( IrReceiver.decode()) {
      String irNum = String(IrReceiver.decodedIRData.command,DEC);
       int irNumb;
       irNumb = irNum.toInt();
      Serial.println(irNumb);
      switch (irNumb) {
        
         case 24:            // button 2 FORDWARD
            forward();
            break;

         case 8 :            // button 4 LEFT
            left();
            break;
    
         case 90:            // button 6 RIGHT
            right();
            break;
 
         case 82:            // button 8 BACK
            back();
            break;
          case 0:
          IrReceiver.resume(); // when read a 0, that means that you are pressing too long one button it starts again with the reading 
          break;
        case 70:    // button 2 STOP
           Stop();
           break;
         
         }      
   IrReceiver.resume();
   }
   
   
}

void forward()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena, 255);
  Serial.println("FRONT");
  delay(100);
  Stop();
            
            
}

void back()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena, 255);
  delay(100);
  Stop();
 Serial.println("BACK");
     
      
}

void left()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(ena, 255); // half of the speed 
  delay(100);
  Stop();
  Serial.println("left");
      
      
}

void right()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena, 255); // half of the speed 
  delay(100);
  Stop();
  Serial.println("right");
      
}


void Stop()
{
  analogWrite(ena, 255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}