
#include <Wire.h>
#include <string.h>



//---------------------------------------- PIN settings-------------- 

const int Left_Forward_buttonPin = 3;     // the number of the pushbutton pin
const int Left_Back_buttonPin = 5;     // the number of the pushbutton pin
const int Right_Forward_buttonPin = 7;     // the number of the pushbutton pin
const int Right_Back_buttonPin = 4;     // the number of the pushbutton pin
const int Up_buttonPin = 6;     // the number of the pushbutton pin
const int Down_buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  9;      // the number of the LED pin
int num_pins = 6;
const int pins[6] = {Left_Forward_buttonPin,Left_Back_buttonPin,Right_Forward_buttonPin, Right_Back_buttonPin, Up_buttonPin,Down_buttonPin}; 
int pins_value[6] = {0,0,0,0,0,0};

//--------------------------------

String output_value;
String buttonState_value;
String outMessage = "";
char message[31];
float actual_temp = 0;
float voltage;
float temperature;
 
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status


//---------------------------------------
void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  
  // initialize the pushbutton pin as an input:
  for  (int i = 0; i < num_pins; i++) {  
     pinMode(pins[i], INPUT);   
     Serial.println(i);
  }
   
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.println("starting...");
}






void loop()
{

  send_pin_state(); 
  delay(200);

//  request_data(); // get temperature and voltage 
  
  Serial.print("Thruster status: ");
  for  (int i = 0; i < num_pins; i++) {
    Serial.print( pins_value[i]);
  }
  Serial.print("  Voltage: ");
  Serial.print(voltage,1);
  Serial.print("  Temp: "); 
  Serial.println(temperature,1);
  delay(200);
  
}


void request_data (void) {

  Wire.requestFrom(4,12); 
  String stringOne = "";
  String temperature_str;
  String  voltage_str;
 
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
   // Serial.print(c);         // print the character
     String str = String(c);
    if (str == ",")  {
      temperature_str = stringOne.substring(0,3);
      stringOne = "";
    } else {
       stringOne += c;
    }
  
  }
  
   voltage_str = stringOne.substring(0,2);
 
   voltage = (voltage_str.toInt());
   voltage = voltage/10;  
     
   temperature = (temperature_str.toInt()); 
   temperature = temperature/10;
   
}


void send_pin_state (void) {
     delay(50);
    Wire.beginTransmission(4); // transmit to device #4
    // get pin values 
    delay(150);
    buttonState_value ="";
    // we need to get the pin values and append them to the output string 
    output_value ="<";
    for  (int i = 0; i < num_pins; i++) {
        // read the state of the pushbutton value:
        int buttonState = digitalRead(pins[i]);
        // check if the pushbutton is pressed.
        // if it is, the buttonState is HIGH:
   
        if (i != 0)  { 
           output_value +=",";
        }  
    
        if ((buttonState == LOW) ) { //or  (     (    (i == 2) or (i ==4) or (i == 6)    ) and  (pins_value[i-1] == 1)   and (buttonState == HIGH)   )    ) {
            buttonState_value = String(1);
            pins_value[i] =1; 
        } 
        else {
           buttonState_value = String(0);
           pins_value[i] =0; 
      
         }
        output_value += buttonState_value;
    }  
    output_value +=">";
   
    char sendStatus[17];
    output_value.toCharArray(sendStatus, 17);
    
    Wire.write(sendStatus);
    delay(350);
    Wire.endTransmission(4);    // stop transmitting
    delay(350);
   // Serial.print(sendStatus);
}

