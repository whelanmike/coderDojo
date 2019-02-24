#include <SoftwareSerial.h>
// BLE-4 module config (HM-10)
//int Tx 8;
//int Rx 9;
SoftwareSerial mySerial(8, 9); // Tx, Rx
String bleString ;

//int RELAY_1 = 7; //Declaring the pins where in1 in2 from the driver are wired
//int RELAY_2 = 3;
int RELAY_3 = 2;
int RELAY_4 = 4;
int RPWM_output = 5;
int LPWM_output = 6;
int led = 13 ;
int duration = 7000;
unsigned long time_now = 0;
int voltage = 00;
int stepper = 0;
unsigned long elapsed = 0;
char dirn = 'A';
char c = ' ';

void setup() {
  Serial.begin(115200);
//  pinMode(RELAY_1, OUTPUT); //Declaring the pin modes, obviously they're outputs
//  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RPWM_output, OUTPUT);
  pinMode(LPWM_output, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(13, LOW);
//  digitalWrite(RELAY_1, HIGH);  // OFF
//  digitalWrite(RELAY_2, HIGH);  // OFF
  digitalWrite(RELAY_3, HIGH);  // OFF
  digitalWrite(RELAY_4, HIGH);  // OFF
  mySerial.begin(9600);
  delay(100);
}

void loop(){
  if (mySerial.available()>0){
    c = mySerial.read();
    Serial.write(c);
  }
  if (c == 'X'){
    PowerOn();
    }
  if (c == 'Y'){
    PowerOff();
    }

  if (c == 'F' && dirn != 'F' && dirn != 'R'){
    time_now = millis();
    stepper = 0;
    voltage = 0;
    dirn = 'F';
    analogWrite (RPWM_output, 0);
    }
  if (c == 'R' && dirn != 'R' && dirn != 'F'){
    time_now = millis();
    stepper = 0;
    voltage = 0;
    dirn = 'R';
    analogWrite (LPWM_output, 0);
    }
    
  elapsed = millis() - time_now;
  
  if (dirn == 'F'){     // Forward Direction  
    startMotor ('F', LPWM_output);
  }

  if (dirn == 'R' ){    //  -- Reverse direction ..
    startMotor ('R', RPWM_output);
  }
}
 
void PowerOn(){
  digitalWrite(RELAY_3, LOW);
  digitalWrite(RELAY_4, LOW);
}

void PowerOff(){
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
}
void startMotor (char dirn, int PWM_dirn){
  if (elapsed < duration){
    analogWrite (PWM_dirn, voltage);
    if (stepper <= 2000){
      voltage += 1;
      if ( voltage > 254 ){
        voltage = 255;
      }
      stepper += 1;
    }
  }
  if (millis() - time_now >= duration){
    voltage -= 1;
    analogWrite (PWM_dirn, voltage);
    if ( stepper > 0 ){
      stepper -= 1;
      voltage -= 1;
      if ( voltage < 0){
        stepper = 0;
      }
    }
    if (stepper <= 0){
      dirn = 'A';   // Flag Stopped.
    }
  }
}
