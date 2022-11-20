int GREEN = 2;        // Wire Pin 2 to Green LED
int YELLOW = 3;       // Pin 3 to Yellow LED
int RED = 4;          // Pin 4 to Red LED
int buttonPin = 5;    // Button to detect pedestrian crossing.
int buttonState = 0;  // variable to tell if button has been pressed on not. 

void setup() {  // put your setup code here, to run once:
  pinMode(GREEN, OUTPUT);         // All LEDs are output.  We send power out to turn them on.
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(buttonPin, INPUT);      // Pedestrian button is read in by the controller.
}

void loop() {// put your main code here, to run repeatedly:
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  delay(2000);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
  delay(1000);
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  delay(2000);
  
  buttonState = digitalRead (buttonPin);
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH){
    for (int i=0; i<50; i++){
      flashLights();
    }
    digitalWrite(RED, HIGH);
    delay(3000);      
  }
     
}


void flashLights(){
  digitalWrite(GREEN, HIGH);
  delay(100);      
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
  delay(100);      
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  delay(100);      
  digitalWrite(RED, LOW);
  delay(10);
} 
