#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <DS3232RTC.h>
#include <SD.h>

#include "Wire.h"
#define DS3232_I2C_ADDRESS 0x68

#define interruptPin 2 //Pin we are going to use to wake up the Arduino  interrupt pin (D2)
#define NOP __asm__("nop\n\t")

const unsigned int sd_chipSelect      = 6;
char               charFileName[16]   = "";
String             logString;
File               logFile;
float              temperature_float  = 0.0;
char               temperature_char[10];
String             logTime;
const byte         sleep_for_mins     = 1; // Sets the wake up intervall in minutes
unsigned int       Year, Month, Day, Hour, Minute, Second;  // Function to update these values.
boolean            isPumping          = false;
const unsigned int repeatCheckMinutes = 1;
unsigned long      currentTime        = 0;
unsigned long      lastCheckTime      = 0 ;
unsigned int       sleepTimeSeconds   = 30; //* 60; //15;
unsigned int       currentPressure    = 0;
float              voltage            = 0.0;
const float        voltageScaling     = 0.01873835614;   // Measured.
const float        lowVoltageThreshold = 11.0;
unsigned int       previousPressure   = 0;
const unsigned int pressureLow        = 230;
const unsigned int pressureHigh       = 350;
const unsigned int pressureDelta      = 0;  // Indicates that tank is filling.
const unsigned int relayPin           = 8; //D8 pin -- connected to relay.
time_t t;

DS3232RTC RTC;

void setup()
{
//    Wire.begin();   // REM
    Serial.begin(57600);        // Starts serial communication in 9600 baud rate.
    RTC.begin();
    detachInterrupt(0);    
    setSyncProvider(RTC.get);  // Library function to get the time from the RTC module.
    if (timeStatus() != timeSet)
      {
        Serial.println("System Time Cannot be Set. Check Connections.");
        Serial.println(timeStatus());
        Serial.println(RTC.get()+7UL*3600UL);
      }
    else
    {
      Serial.println("System Time is Set.");
      Serial.println(RTC.get()+7UL*3600UL);
    }
    displayAlarm1();
    Serial.println("Initializing SD card...");
    
    // see if the card is present and can be initialized:
    if (!SD.begin(sd_chipSelect)) {
      Serial.println("initialization failed!");
      while (1);
    }
    else{
      Serial.println("SD Card OK");
    }

    Serial.println("initialization done.");
    
    pinMode(LED_BUILTIN, OUTPUT);         //We use the led on pin 13 to indicate when Arduino is asleep
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);
    pinMode(interruptPin, INPUT_PULLUP);  //Set pin d2 to input using the builtin pullup resistor
    digitalWrite(LED_BUILTIN, HIGH);      //turning LED on

    RTC.setAlarm(DS3232RTC::ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(DS3232RTC::ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(DS3232RTC::ALARM_1);
    RTC.alarm(DS3232RTC::ALARM_2);
    RTC.alarmInterrupt(DS3232RTC::ALARM_1, false);
    RTC.alarmInterrupt(DS3232RTC::ALARM_2, false);
    RTC.squareWave(DS3232RTC::SQWAVE_NONE);          // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    
    time_t t;                             //create a temporary time variable so we can set the time and read the time from the RTC
    t = RTC.get();                        //Gets the current time of the RTC
    RTC.setAlarm(DS3232RTC::ALM1_MATCH_SECONDS, (second(t) + sleepTimeSeconds) % 60, 0, 0, 0); //  USE for testing TRIGGER every few seconds.
//    RTC.setAlarm(DS3232RTC::ALM1_MATCH_MINUTES , 0, ((minute(t) + sleep_for_mins)%60)+00001, 0, 0); // Setting alarm 1 to go off X minutes from now. (Denis Barrett Fix!! using MOD) PROD ***********
//    Serial.print ((minute(t) + sleep_for_mins) % 60)+1; Serial.println(" <==== Alarm Minute First setting");
    RTC.alarm(DS3232RTC::ALARM_1);                   // clear the alarm flag
    RTC.squareWave(DS3232RTC::SQWAVE_NONE);
    RTC.alarmInterrupt(DS3232RTC::ALARM_1, true);    // enable interrupt output for Alarm 1
      //  displayAlarm1();
//   displayAlarm2();
    Serial.println('End of setup()');
}

void loop()
{
    if (! isPumping){
        // Serial.println("isPumping   -------<NO>");
        updateTime();
        sprintf (charFileName, "%04d%02d%02d.log", Year, Month, Day);

        temperature_float = RTC.temperature() / 4.0;
        dtostrf (temperature_float, 4, 2, temperature_char);  // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
        currentPressure = readPressure();
        voltage = readSupplyVoltage();        
        Serial.print("Current Pressure NOT Pumping :: "); Serial.println(currentPressure);

        logString = (displayTime());
        logString += "|" + String(temperature_char) +"|";
        logString += String (currentPressure) + "|";
        logString += String (voltage) + "|";
        // Serial.println(logString);
        logFile = SD.open(charFileName, FILE_WRITE);   // This has to be a character array.
        if ( logFile ) {
          logFile.println (logString);
          logFile.close();
        }
        else {                            // if the file isn't open, pop up an error:
          Serial.println("error opening file :- " + String(charFileName));
        }

        if (currentPressure < pressureLow && voltage > lowVoltageThreshold){   // && voltage > lowVoltageThreshold HERE!!!
            Serial.println ("LOW Pressure. Set isPumping ON ");
            setPumping (true);
            }
        else{
          // }
            delay(1000);       // 5 sec delay  **** GET RID OF THIS
            t = RTC.get();
            Going_To_Sleep();
        }
      }
    else{
        updateTime();
        // Serial.println("isPumping   -------<YES>");
        if (aMinuteSinceLastReading()){
            currentPressure = readPressure(); 
            voltage = readSupplyVoltage();
            temperature_float = RTC.temperature() / 4.0;
            dtostrf (temperature_float, 4, 2, temperature_char);  // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
            sprintf (charFileName, "%04d%02d%02d.log", Year, Month, Day);

            logString = (displayTime());
            logString += "|" + String(temperature_char) +"|";
            logString += String (currentPressure) + "|";
            logString += String (voltage) + "|";
            // Serial.println(logString); 
            logFile = SD.open(charFileName, FILE_WRITE);   // This has to be a character array.
            if ( logFile ) {
              logFile.println (logString);
              logFile.close();
            }
            else {                            // if the file isn't open, pop up an error:
              Serial.println("error opening file :- " + String(charFileName));
            }

            if (currentPressure > pressureHigh || voltage <= lowVoltageThreshold){  // || voltage <= lowVoltageThreshold  HERE!!!
                Serial.println ("HIGH Pressure or Low Voltage. Set isPumping OFF ");
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
        lastCheckTime = millis();
        }
        else{                             // if a reading has been taken recently, do nothing.
            NOP;
        }
    delay(500);
    }
    // Serial.println("End of main() loop,");
    // previousPressure = currentPressure;
}
/* END loop . Define functions after this */

void updateTime()                         // Function that prints the time to serial monitor.
{
    // time_t t;
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
    Serial.println("Going to Sleep function()");
    t = RTC.get();                        //gets current time from rtc
//    Serial.println("Sleep  Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //prints time stamp on serial monitor
    RTC.alarm(DS3232RTC::ALARM_1);                   // clear the alarm flag
    RTC.setAlarm(DS3232RTC::ALM1_MATCH_SECONDS, (second(t) + sleepTimeSeconds) % 60, 0, 0, 0); //  USE for testing TRIGGER every few seconds.
    // Serial.print("---------------------------------------------------------seconds alarm matching-> ");
    // Serial.println ((second(t) + 10) % 60);
//    Serial.print ((minute(t) + sleep_for_mins) % 60); Serial.println(" <==== Alarm Minute setting");
    // RTC.squareWave(DS3232RTC::SQWAVE_NONE);
    RTC.alarmInterrupt(DS3232RTC::ALARM_1, true);    // enable interrupt output for Alarm 1
    sleep_enable();                       //Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);      //attaching an interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN, LOW);       //turning LED off
    // time_t t;                             // creates temp time variable

  // Serial.print ("Alarm_1 time2 ====> ");
  // displayAlarm1();

    delay(1000);                          //wait a second to allow the led to be turned off before going to sleep
    // sei();
    sleep_cpu();                          //activating sleep mode

    // Serial.println("just woke up!");      //next line of code executed after the interrupt
    digitalWrite(LED_BUILTIN, HIGH);      //turning LED on
  
//    Serial.println("Wake Up Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //Prints time stamp
  
    //Set New Alarm
//    RTC.setAlarm(DS3232RTC::ALM1_MATCH_MINUTES , 21, ((minute(t) + sleep_for_mins) % 60), 21, 0); // PROD ALARM_1 config
//   displayAlarm2();
    
}

void wakeUp() {
    Serial.println("Wake Up function() [Interrupt]");   //Print message to serial monitor
    sleep_disable();                      //Disable sleep mode
    detachInterrupt(0);                   //Removes the interrupt from pin 2;
}

void setPumping (boolean onOff){
    isPumping = onOff;
    if (isPumping){
        digitalWrite (relayPin, LOW);
    }
    else{
        digitalWrite (relayPin, HIGH);
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
    if (second(t) % 5 ==0 ){              // This is just for debugging REMOVE in PROD
    Serial.println("Read Pressure function()"); 
    }
    delay(100);
    return analogRead (A0);
}

float readSupplyVoltage(){              // Voltage divider output connected to A2
    delay(100);
    return ( (float) analogRead (A2) * voltageScaling );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  // return ( (val/16*10) + (val%16) );
  return ( (val >> 4) * 10 + (val & 0x0F) );
}

void readDS3232alarm1(byte *second, 
byte *minute, 
byte *hour, 
byte *dayOfMonth)
{
  Wire.beginTransmission(DS3232_I2C_ADDRESS);
  Wire.write(0x07); // set DS3232 register pointer to 07h
  Wire.endTransmission();  
  Wire.requestFrom(DS3232_I2C_ADDRESS, 4); // request 4 bytes of data from DS3232 starting from register 07h

  *second     = bcdToDec(Wire.read());
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);
  *dayOfMonth = bcdToDec(Wire.read()) ;
}

void displayAlarm1(){
  byte second, minute, hour, dayOfMonth;
  
  // retrieve data from DS3232  
  readDS3232alarm1(&second, &minute, &hour, &dayOfMonth);
  
  // send it to the serial monitor
  Serial.print("Alarm One: ");
  Serial.print(hour);// convert the byte variable to a decimal number when being displayed
  Serial.print(":");
  if (minute<10)
  {
      Serial.print("0");
  }
  Serial.print(minute);
  Serial.print(":");
  if (second<10)
  {
      Serial.print("0");
  }
  Serial.print(second);
  Serial.print(" == ");
  Serial.println(dayOfMonth);
}
