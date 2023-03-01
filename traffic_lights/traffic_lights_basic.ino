int GREEN = 2;        // Wire Pin 2 to Green LED
int YELLOW = 3;       // Pin 3 to Yellow LED
int RED = 4;          // Pin 4 to Red LED

void setup() {  // put your setup code here, to run once:
 pinMode(GREEN, OUTPUT);         // All LEDs are output.  We send power out to turn them on.
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
