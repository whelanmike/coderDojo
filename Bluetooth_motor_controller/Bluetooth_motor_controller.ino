#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(8, 9); // Tx, Rx

//int RELAY_1 = 7; //Declaring the pins where in1 in2 from the driver are wired
//int RELAY_2 = 3;
int RELAY_3 = 2;
int RELAY_4 = 4;
int RPWM_output = 5;
int LPWM_output = 6;
int led = 13 ;
int duration = 7000;
unsigned long time_start = 0;
int voltage = 0;
unsigned long elapsed = 0;
char dirn = 'A';
char c = ' ';

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RPWM_output, OUTPUT);
  pinMode(LPWM_output, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(RELAY_3, HIGH);  // OFF
  digitalWrite(RELAY_4, HIGH);  // OFF
  bluetoothSerial.begin(115200);
  delay(100);
}

void loop(){
  if (bluetoothSerial.available()>0){
    c = bluetoothSerial.read();
    Serial.write(c);
  }
  if (c == 'X'){
    PowerOn();
    }
  if (c == 'Y'){
    PowerOff();
    }

  if (c == 'F' && dirn != 'F' && dirn != 'R'){
    initVars();
    analogWrite (RPWM_output, 0);
    }
  if (c == 'R' && dirn != 'R' && dirn != 'F'){
    initVars();
    analogWrite (LPWM_output, 0);
    }

  elapsed = millis() - time_start;

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
void initVars(){
  time_start = millis();
  voltage = 0;
  dirn = c;
}
void startMotor (char dirn, int PWM_dirn){
  if (elapsed < duration){
    analogWrite (PWM_dirn, voltage);
      voltage += 1;
      if ( voltage > 254 ){
        voltage = 255;
      }
  }
  if (elapsed >= duration){
    voltage -= 1;
    analogWrite (PWM_dirn, voltage);
      voltage -= 1;
      if ( voltage <= 0){
        dirn = 'A'; // Flag STOP
      }
  }
}
