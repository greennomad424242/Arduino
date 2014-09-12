
#include <Wire.h>
#include <string.h>


const int Left_Forward_buttonPin = 2;     // the number of the pushbutton pin
const int Left_Back_buttonPin = 3;     // the number of the pushbutton pin
const int Right_Forward_buttonPin = 4;     // the number of the pushbutton pin
const int Right_Back_buttonPin = 12;     // the number of the pushbutton pin
const int Up_buttonPin = 13;     // the number of the pushbutton pin
const int Down_buttonPin = 14;     // the number of the pushbutton pin
const int ledPin =  7;      // the number of the LED pin
String buttonState_value;
String outMessage = "24554545";
char message[25];

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int num_pins = 3;
const int pins[6] = {Left_Forward_buttonPin,Left_Back_buttonPin,Right_Forward_buttonPin, Right_Back_buttonPin, Up_buttonPin,Down_buttonPin}; 
int pins_value[6];

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  for  (int i = 0; i < num_pins; i++) {  
     pinMode(pins[i], INPUT);   
  }
   
 Wire.begin(); // join i2c bus (address optional for master)
  Serial.println("starting...");
}



void loop()
{
  Serial.print("reading:");
  String output_value;
  Wire.beginTransmission(4); // transmit to device #4
  buttonState_value ="";
  output_value ="";
  for  (int i = 0; i < num_pins; i++) {
     
    
    // read the state of the pushbutton value:
   int buttonState = digitalRead(pins[i]);
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
   
    if (i != 0)  { 
      output_value +=",";
    }  
    
    if (   (buttonState == LOW) ) { //or  (     (    (i == 2) or (i ==4) or (i == 6)    ) and  (pins_value[i-1] == 1)   and (buttonState == HIGH)   )    ) {
       buttonState_value = String(0);
       pins_value[i] =0; 
    } 
    else {
    
       buttonState_value = String(1);
       pins_value[i] =1; 
      
    }
    output_value += buttonState_value;
  }  
  char sendStatus[31];
   output_value.toCharArray(sendStatus, 31);
 // sprintf(message, "[%s]", output_value); // size of this string can vary from 10 up to 15
  Wire.write(sendStatus);
  Serial.println(  output_value ); 
  Wire.endTransmission(4);    // stop transmitting
  delay(200);
  
}
