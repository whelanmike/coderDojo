int GREEN = 2;
int YELLOW = 3;
int RED = 4;
int buttonPin = 5;
int buttonState = 0;

void setup() {  // put your setup code here, to run once:
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(buttonPin, INPUT);
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
