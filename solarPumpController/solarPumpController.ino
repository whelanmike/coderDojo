#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <DS3232RTC.h>
#include <SD.h>

#define interruptPin 2 //Pin we are going to use to wake up the Arduino  interrupt pin (D2)
#define NOP __asm__("nop\n\t")

const unsigned int sd_chipSelect      = 6;
char               charFileName[16]   = "";
String             logString          = "";
File               logFile;
float              temperature_float  = 0.0;
char               temperature_char[10];
String             logTime;
const byte         sleep_for_mins     = 1; // Sets the wakeup intervall in minutes
unsigned int       Year, Month, Day, Hour, Minute, Second;  // Function to update these values.
boolean            isPumping          = false;
const unsigned int repeatCheckMinutes = 1;
unsigned long      currentTime        = 0;
unsigned long      lastCheckTime      = 0 ;
unsigned int       sleepTimeMinutes   = 15;
unsigned int       currentPressure    = 0;
unsigned int       previousPressure   = 0;
const unsigned int pressureLow        = 150;
const unsigned int pressureHigh       = 200;
const unsigned int pressureDelta      = 3;  // Indicates that tank is filling.
unsigned int       batteryVoltage;
unsigned int       panelVoltage;
const unsigned int relayPin           = 0; //TBD

void setup()
{
    Serial.begin(9600);        // Starts serial communication in 9600 baud rate.
    
    setSyncProvider(RTC.get);  // Library function to get the time from the RTC module.
    if (timeStatus() != timeSet)
      Serial.println("System Time Cannot be Set. Check Connections.");
    else
      Serial.println("System Time is Set.");
    
    Serial.print("Initializing SD card...");
    
    // see if the card is present and can be initialized:
    if (!SD.begin(sd_chipSelect)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
    
    pinMode(LED_BUILTIN, OUTPUT);         //We use the led on pin 13 to indicate when Arduino is asleep
    pinMode(interruptPin, INPUT_PULLUP);  //Set pin d2 to input using the builtin pullup resistor
    digitalWrite(LED_BUILTIN, HIGH);      //turning LED on

    //  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.squareWave(SQWAVE_NONE);          // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    
    time_t t;                             //create a temporary time variable so we can set the time and read the time from the RTC
    t = RTC.get();                        //Gets the current time of the RTC
    RTC.setAlarm(ALM1_MATCH_MINUTES , 0, (minute(t) + sleep_for_mins) % 60, 0, 0); // Setting alarm 1 to go off X minutes from now. (Denis Barrett Fix!! using MOD)
    RTC.alarm(ALARM_1);                   // clear the alarm flag
    RTC.alarmInterrupt(ALARM_1, true);    // enable interrupt output for Alarm 1
}

void loop()
{
    if (! isPumping){
        updateTime();
        sprintf (charFileName, "%04d%02d%02d.log", Year, Month, Day);
        Serial.print (String (charFileName)); Serial.println( "              | file name |     ");
        temperature_float = RTC.temperature() / 4.0;
        dtostrf (temperature_float, 4, 2, temperature_char);  // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
        if (aMinuteSinceLastReading){
            currentPressure = readPressure();
            if (currentPressure < pressureLow){
                Serial.println ("LOW Pressure. Set isPumping ON ");
                setPumping (true);
            }
          }
        logString = (displayTime()) +"|";
        logString += String(temperature_char) +"|";
        logString += String (currentPressure);
        logFile = SD.open(charFileName, FILE_WRITE);   // This has to be a character array.
        if ( logFile ) {
          logFile.println (logString);
          Serial.print (logString); Serial.println (" | log data  | ");
          logFile.close();
        }
        else {                            // if the file isn't open, pop up an error:
          Serial.println("error opening file :- " + String(charFileName));
        }
//        previousPressure = currentPressure;
//        lastCheckTime = millis();
        delay(5000);       // 5 sec delay
        Going_To_Sleep();
      }
    else{
        if (aMinuteSinceLastReading){
            currentPressure = readPressure();
            if (currentPressure > pressureHigh){
                Serial.println ("HIGH Pressure. Tank full. Set isPumping OFF ");
                setPumping (false);
            }
            else{
                if ((currentPressure - previousPressure) < pressureDelta){ // Tank NOT filling.
                    Serial.println ("Pressure delta too low. Tank not filling. Set isPumping OFF ");
                    setPumping (false);
                }
                else{
                    previousPressure = currentPressure;
                }
            }
        }
        else{                             // if a reading has been taken recently, do nothing.
            NOP;
        }
    }
    lastCheckTime = millis();
}

/* END loop . Define functions after this */
void updateTime()                         // Function that prints the time to serial monitor.
{
    time_t t;
    t = RTC.get();                        // Get the current time.
    Year = year(t);                       // Update the following GLOBAL vars
    Month = month(t);
    Day = day(t);
    Hour = hour(t);
    Minute = minute(t);
    Second = second(t);
}

String displayTime(){                     // return timestamp (YYYY-MM-DD HH:MM:SS) as character array.
    char current_timestamp[20];
    sprintf(current_timestamp, "%4d-%02d-%02d %02d:%02d:%02d", Year, Month, Day, Hour, Minute, Second);
    return current_timestamp;
}

void Going_To_Sleep() {
    sleep_enable();                       //Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);      //attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN, LOW);       //turning LED off
    time_t t;                             // creates temp time variable
    t = RTC.get();                        //gets current time from rtc
    Serial.println("Sleep  Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //prints time stamp on serial monitor
    delay(1000);                          //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();                          //activating sleep mode
    Serial.println("just woke up!");      //next line of code executed after the interrupt
    digitalWrite(LED_BUILTIN, HIGH);      //turning LED on
  
    t = RTC.get();
    Serial.println("WakeUp Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //Prints time stamp
  
    //Set New Alarm
    RTC.setAlarm(ALM1_MATCH_MINUTES , 0, (minute(t) + sleep_for_mins) % 60, 0, 0);
    RTC.alarm(ALARM_1);                   // clear the alarm flag
}

void wakeUp() {
    Serial.println("Interrrupt Fired");   //Print message to serial monitor
    sleep_disable();                      //Disable sleep mode
    detachInterrupt(0);                   //Removes the interrupt from pin 2;
}

void setPumping (boolean onOff){
    isPumping = onOff;
    if (isPumping){
        digitalWrite (relayPin, HIGH);
    }
    else{
        digitalWrite (relayPin, LOW);
    }
}

boolean aMinuteSinceLastReading (){
    long rightnow = millis();
    if (rightnow > (lastCheckTime + (repeatCheckMinutes * 60UL * 1000))){ // min ->sec -> millis
        return true;
    }
    else{
        return false;
    }
}

unsigned int readPressure(){              // Pressure Sensor on A0
    return analogRead (A0);
}
