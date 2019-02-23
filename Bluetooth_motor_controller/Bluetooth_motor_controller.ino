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
//if (Serial.available()>0){
//Serial.write(mySerial.read());
//}
if (mySerial.available()>0){
  c = mySerial.read();
  Serial.write(c);
}
  if (c == 'X'){
    PowerOn();
    }
  if (c == 'Y'){
//    Serial.println ("------------> powering OFF now.......  : ");
    PowerOff();
    }

  if (c == 'F' && dirn != 'F' && dirn != 'R'){
//    Serial.println ("------------> the start bit : ");
    time_now = millis();
    stepper = 0;
    voltage = 0;
    dirn = 'F';
    }
  if (c == 'R' && dirn != 'R' && dirn != 'F'){
//    Serial.println ("------------> the start bit : ");
    time_now = millis();
    stepper = 0;
    voltage = 0;
    dirn = 'R';
    }
    
  elapsed = millis() - time_now;
// Forward Direction  
  if (dirn == 'F' && elapsed < duration){
    analogWrite (RPWM_output, 0);
    analogWrite (LPWM_output, voltage);
//    Serial.println (String("------------------> Speeding up.  V : ") + voltage );
//    Serial.print("Elapsed time: ");
//    Serial.println(elapsed);
    if (stepper <= 2000){
      voltage += 1;
      if ( voltage > 254 ){
        voltage = 255;
      }
      
      stepper += 1;
//      Serial.println (String("direction Init: ") + dirn );
    }
//   Serial.println (String("Stepper : ") + stepper );
//   Serial.println (String("direction : ") + dirn );

  }
  if (dirn == 'F' && millis() - time_now >= duration){
    voltage -= 1;
//    Serial.println (String("------------------> Slowing down. V : ") + voltage );
    analogWrite (LPWM_output, voltage);
    if ( stepper > 0 ){
      stepper -= 1;
      voltage -= 1;
      if ( voltage < 0){
        stepper = 0;
      }
      
//      Serial.println (String("direction  slow: ") + dirn );
      }
     if (stepper <= 0){
//      Serial.println (String("Finished : ") + dirn );
      dirn = 'A';
     }
  }
// End Forward direction.
//  -- Reverse direction ..
  if (dirn == 'R' && elapsed < duration){
    analogWrite (LPWM_output, 0);
    analogWrite (RPWM_output, voltage);
    if (stepper <= 2000){
      voltage += 1;
      if ( voltage > 254 ){
        voltage = 255;
      }
      
      stepper += 1;
    }

  }
  if (dirn == 'R' && millis() - time_now >= duration){
    voltage -= 1;
    analogWrite (RPWM_output, voltage);
    if ( stepper > 0 ){
      stepper -= 1;
      voltage -= 1;
      if ( voltage < 0){
        stepper = 0;
      }

    }
     if (stepper <= 0){
      dirn = 'A';
     }
  }

// End Reverse direction
}
 
void PowerOn(){
  digitalWrite(RELAY_3, LOW);
  digitalWrite(RELAY_4, LOW);
}

void PowerOff(){
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
}

