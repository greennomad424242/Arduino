/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 0;
int led1 = 1;
int led2 = 2;
int led3 = 3;
int led4 = 4;
int led5 = 5;
int led6 = 6;
int led7 = 7;



// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
    pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT);   
  pinMode(led3, OUTPUT);  
   pinMode(led4, OUTPUT);  
    pinMode(led5, OUTPUT);  
     pinMode(led6, OUTPUT);  
      pinMode(led7, OUTPUT);   
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);
  digitalWrite(led1, HIGH);
  
  digitalWrite(led2, HIGH);  
  digitalWrite(led3, HIGH); 
  digitalWrite(led4, HIGH); 
  digitalWrite(led5, HIGH); 
  digitalWrite(led6, HIGH); 
  digitalWrite(led7, HIGH); 

    
     // turn the LED on (HIGH is the voltage level)
  delay(2000);        
  digitalWrite(led, LOW);  
   digitalWrite(led1, LOW);  
  digitalWrite(led2, LOW);   // wait for a second
 
 digitalWrite(led3, LOW);  // turn the LED off by making the voltage LOW
  digitalWrite(led4, LOW);  
  digitalWrite(led5, LOW);  
  digitalWrite(led6, LOW);  
  digitalWrite(led7, LOW);   
  
  
  delay(2000);               // wait for a second
}
