#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <DS3232RTC.h>
#include <SPI.h>
#include <SD.h>

#define interruptPin 2 //Pin we are going to use to wake up the Arduino  interrupt pin (D2)

const int sd_chipSelect = 6;
char charFileName[16] = "";
String logString = "";
String tempVar;
File logFile;
float temperature_reading = 0.0;
char temperature_str[10];
String logTime;
const int time_interval = 1; // Sets the wakeup intervall in minutes
int Year, Month, Day, Hour, Minute, Second;

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

  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);

  time_t t;   //create a temporary time variable so we can set the time and read the time from the RTC
  t = RTC.get(); //Gets the current time of the RTC
  RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t) + time_interval, 0, 0); // Setting alarm 1 to go off 5 minutes from now
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
  RTC.squareWave(SQWAVE_NONE);
  // enable interrupt output for Alarm 1
  RTC.alarmInterrupt(ALARM_1, true);

}

void loop()
{
  updateTime();
  sprintf (charFileName, "%04d%02d%02d.log", Year, Month, Day);
  Serial.print (String (charFileName)); Serial.println( " <------------------------->");
  temperature_reading = RTC.temperature() / 4.0;
  dtostrf (temperature_reading, 4, 2, temperature_str);  // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
  logString = (displayTime()) +"|";
  logString += String(temperature_str);
  logFile = SD.open(charFileName, FILE_WRITE);   // This has to be a character array.
  if ( logFile ) {
    logFile.println (logString);
    Serial.println (logString);
    logFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file :- " + String(charFileName));
  }

  delay(5000);       // 5 sec delay
  Going_To_Sleep();

}

void updateTime()  // Function that prints the time to serial monitor.
{
  time_t t;
  t = RTC.get();  // go away and get the time.
  Year = year(t);
  Month = month(t);
  Day = day(t);
  Hour = hour(t);
  Minute = minute(t);
  Second = second(t);
}

String displayTime(){
  char current_timestamp[120];
  sprintf(current_timestamp, "%4d-%02d-%02d %02d:%02d:%02d", Year, Month, Day, Hour, Minute, Second);
//  Serial.print (current_timestamp);
//  Serial.println (" Formatted Time ");
  return current_timestamp;
}

void Going_To_Sleep() {
  sleep_enable();//Enabling sleep mode
  attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
  digitalWrite(LED_BUILTIN, LOW); //turning LED off
  time_t t;// creates temp time variable
  t = RTC.get(); //gets current time from rtc
  Serial.println("Sleep  Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //prints time stamp on serial monitor
  delay(1000); //wait a second to allow the led to be turned off before going to sleep
  sleep_cpu();//activating sleep mode
  Serial.println("just woke up!");//next line of code executed after the interrupt
  digitalWrite(LED_BUILTIN, HIGH); //turning LED on

  t = RTC.get();
  Serial.println("WakeUp Time: " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t))); //Prints time stamp
  //Set New Alarm
  RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t) + time_interval, 0, 0);

  // clear the alarm flag
  RTC.alarm(ALARM_1);
}

void wakeUp() {
  Serial.println("Interrrupt Fired");//Print message to serial monitor
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
}
