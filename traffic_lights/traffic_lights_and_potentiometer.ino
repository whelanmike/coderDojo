// Traffic lights sample
int GREEN = 2;        // Wire Pin 2 to Green LED
int YELLOW = 3;       // Pin 3 to Yellow LED
int RED = 4;          // Pin 4 to Red LED

void setup() {  // put your setup code here, to run once:
 pinMode(GREEN, OUTPUT);         // All LEDs are output.  We send power out to turn them on.
 Serial.begin(9600);           //  Allows you to write to the Serial Monitor.
 pinMode(YELLOW, OUTPUT);
 pinMode(RED, OUTPUT);
}

void loop() { // put your main code here, to run repeatedly:
 digitalWrite(GREEN, HIGH);
 digitalWrite(RED, LOW);
 delay(2000);
 digitalWrite(YELLOW, HIGH);
 digitalWrite(GREEN, LOW);
 delay(1000);
 digitalWrite(RED, HIGH);
 digitalWrite(YELLOW, LOW);
 delay(2000);
 }
 
// --------- Potentiometer : voltage measurement 
int analoguePin = 0;  // Wire Pin A0 to Middle pin of Potentiometer.  (0 => analogue pin 0)
int voltageVal = 0;   // The value read by A0 (0 -> 1,023). Note this is an Integer.
float voltage = 0.0 ; // Variable to hold calculated Voltage value. This will be a floating point number.

void loop() {
  voltageVal = analogRead(analogPin);  // read the input pin
  Serial.println(voltageVal);          // debug value
  delay (2000)            // Sleep for 2 seconds
}

// -- if statement in C++
if (voltageVal < 300){
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    }
if (voltageVal < 600){
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    }
if (voltageVal < 900){
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
    }    
else{
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, HIGH);
    }
