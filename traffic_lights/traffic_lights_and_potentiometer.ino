// Traffic lights sample
const int GREEN = 2;        // Wire Pin 2 to Green LED
const int YELLOW = 3;       // Pin 3 to Yellow LED
const int RED = 4;          // Pin 4 to Red LED
// --------- Potentiometer : voltage measurement 
const int analoguePin = 0;  // Wire Pin A0 to Middle pin of Potentiometer.  (0 => analogue pin 0)
int voltageVal = 0;   // The value read by A0 (0 -> 1,023). Note this is an Integer.
float voltage = 0.0 ; // Variable to hold calculated Voltage value. This will be a floating point number.
int delay_milliseconds = 1000;

void setup() {  // put your setup code here, to run once:
 pinMode(GREEN, OUTPUT);         // All LEDs are output.  We send power out to turn them on.
 Serial.begin(9600);           //  Allows you to write to the Serial Monitor.
 pinMode(YELLOW, OUTPUT);
 pinMode(RED, OUTPUT);
}

void loop() { // put your main code here, to run repeatedly:
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  voltageVal = analogRead(analoguePin); // read the input pin
  Serial.println(voltageVal);           // debug value
  delay (delay_milliseconds);           // Sleep for a bit
  
  // -- if statement in C++
  if (voltageVal < 100){
    Serial.println("Less Than : 100");
  }
  else if (voltageVal < 300){
      Serial.println("Less Than : 300");
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
      }
  else if (voltageVal < 600){
    Serial.println("Less Than : 600");
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, LOW);
      }
  else if (voltageVal < 900){
    Serial.println("Less Than : 900");
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, HIGH);
      }    
  else{
    Serial.println("Less Than : ELSE");
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, HIGH);
      delay_milliseconds = 500;
      }
  if (voltageVal > 930){
    delay_milliseconds = (1050 - voltageVal) * 3;
  }
  delay (delay_milliseconds);
  delay_milliseconds = 1000;
 }
 

void lightsOnOff(){
  
}

