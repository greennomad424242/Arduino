// Wire Slave Receiver   Matt


#include <Wire.h>
//#include <Time.h>  
#include <string.h>

//----------------------------- config


float voltage_cutoff = -1.0;
unsigned long updated_time, start_time, delta_time;
float voltage = 0, temperature;


//----------------------------- thruster related

boolean safe_mode = false;

int x;
const int num_pins = 6;
int thruster_array[num_pins];
const int Left_Forward_buttonPin = 3;     // the number of the pushbutton pin
const int Left_Back_buttonPin = 5;     // the number of the pushbutton pin
const int Right_Forward_buttonPin = 7;     // the number of the pushbutton pin
const int Right_Back_buttonPin = 6;     // the number of the pushbutton pin
const int Up_buttonPin = 4;     // the number of the pushbutton pin
const int Down_buttonPin = 8;     // the number of the pushbutton pin
const int power_relay = 2;
const int pins[6] = {Left_Forward_buttonPin,Left_Back_buttonPin,Right_Forward_buttonPin, Right_Back_buttonPin, Up_buttonPin,Down_buttonPin}; 





// thermoistort -------------------------------------------------
// which analog pin to connect
#define THERMISTORPIN A3         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
int samples[NUMSAMPLES];

// constants for the voltage sensing------------------------------------------------------------------
const float referenceVolts = 5;        // the default reference on a 5-volt board
// determine by voltage divider resistors, see text
const float resistorFactor = 320;  

const int batteryPin = 1;   
const int batteryPin2 = 0;  
//  LED 

int red_led = 12;
int green_led = 11;
 
//----------------------------------------

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(green_led, OUTPUT);   
  pinMode(red_led, OUTPUT); 
  for  (int i = 0; i < num_pins; i++) {  
     pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i] , LOW);     
  }
  
   pinMode( power_relay, OUTPUT); 
  digitalWrite( power_relay , LOW);   
  
  start_time = 0;
}

void loop()
{

   updated_time = millis();
   delta_time = updated_time - start_time;

   
 // if (delta_time > 2000.0 || voltage_cutoff > voltage_func())   {
    
  //  digitalWrite(red_led, LOW);   // turn the LED on (HIGH is the voltage level) 
 // } else {
 //   safe_mode = false; 
//    digitalWrite(red_led, LOW);   // turn the LED on (HIGH is the voltage level)
 // }
  
  safe_mode = false;
  
  
  if (safe_mode == false) {
      digitalWrite( power_relay, HIGH);
       for (int i = 0; i < num_pins; i++) {  
           if (thruster_array[i] == 1)  {
               digitalWrite(pins[i] , HIGH);
           } else { 
               digitalWrite(pins[i] , LOW);
           }
        
        } 
        
   } else {
    
   // set all the thrusters to zero  
       for (int i = 0; i < num_pins; i++) {  
           digitalWrite(pins[i] , LOW);
       } 
  }
  
  
  
    
 boolean verbose = true;
  // this allows for easy reporting
 if (verbose == true) {
    
    if (safe_mode == true) {   
       Serial.print("Safe Mode ");
    } else {
     Serial.print("Active Mode ");
    }
    Serial.print(" Delta time:  ");
    Serial.print(delta_time);
    
    Serial.print(" Thrusters state: "); 
    for  (int i = 0; i < num_pins; i++) {
       Serial.print(thruster_array[i]);
       Serial.print(" ");
    }
    Serial.print("  Voltage cutoff: ");
    Serial.print(voltage_cutoff);
    Serial.print("  Voltage: ");  
   Serial.print(voltage_func());
    Serial.print(" Time:");
    Serial.println(delta_time); 
   }
  delay(200);
 // Serial.println("Looping");
  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()




void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    String str = String(c);
    if (str == "<")  {  
        x= 0;
    } else if (str == ",")  {
    
    } else  {
       thruster_array[x] = str.toInt();
       x++;
    }
  }
   int x = Wire.read();    // receive byte as an integer
 // Serial.print(" ");
  //Serial.print("received");
  start_time = millis();
  
}


void requestEvent()
{
  String output_value;
  char sendStatus[10];
  int temp_now = (int) (temp()*10);
  output_value = String(temp_now);
  output_value += ",";
  output_value +=String((int) (voltage_func()*10));  
  //output_value += "|";
  //if (safe_mode == true) {  
  //    output_value += "1";
 //  
  // } else {
   //  output_value += "0";
  // }   
       
  
  output_value.toCharArray(sendStatus, 13);
  Wire.write(sendStatus); // respond with message of 6 bytes
                       // as expected by master
  //Serial.println(output_value);                     
}





float voltage_func ()  {
   int val = analogRead(1);  // read the value from the sensor
   float volts = (val / resistorFactor) * referenceVolts ; // calculate the ratio

   return volts;  // print the value in volts   
}






float temp(void)  {
  uint8_t i;
  float average;
  float fahrenheit;
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
 // Serial.print("Average analog reading "); 
 // Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;

 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  fahrenheit = steinhart * 9/5 + 32;
  temperature = fahrenheit;
  return fahrenheit;
}

