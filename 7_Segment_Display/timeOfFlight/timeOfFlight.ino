int topSensor = 0;
int bottomSensor = 1;
int startTrigger =0;    // Trigger when read to start timing. Top sensor goes dark first.
int endTrigger =0;      // Bottom sensor has detected falling object
int topVal = 0;         // Top photodiode reading
int bottomVal =0;       // Bottom photodiode reading


// timing variables
unsigned long startMillis=0;
unsigned long endMillis=0;
unsigned long currentMillis=0;
unsigned long intervalError=3000;        // If we don't detect a stop event within this time, STOP the clock.

void setup()
{
    Serial.begin(9600);//Set Baud Rate to 9600 bps
}
 void loop()
{   
    topVal=analogRead(topSensor);    // read top photodiode

    if (topVal   < 300) {           // if it goes dark-ish this means something has passed in front of it
        Serial.println("topVal dark");
        Serial.println(topVal);
        startTrigger = 1;
    }
    
    if (topVal > 800 && startTrigger == 1) {   // if it gets bright AFTER being dark, the object has fallen past the sensor.
        Serial.println("topVal starting");
        Serial.println(topVal);
        startTrigger = 0;
        startMillis = millis();             // start the timer.
    }

    bottomVal=analogRead(bottomSensor);          // read bottom photodiode
    if (bottomVal  < 300 && startTrigger == 0) { // if it gets darker, the object is falling past the bottom sensor.
        Serial.println(bottomVal);
        endMillis = millis();                    // record the time.
        endTrigger = 1;
    }

    if (endTrigger=1) {
        unsigned long fallTime=0;
        fallTime = (unsigned long)(startMillis - endMillis);
        Serial.println("fallTime");
        Serial.println(fallTime);
    }
    
    currentMillis = millis();
    if ((unsigned long)(currentMillis - startMillis) >= intervalError) {
        startMillis = 0;
    }

}
