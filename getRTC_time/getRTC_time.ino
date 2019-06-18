#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <DS3232RTC.h>
#include <SPI.h>
#include <SD.h>

#define interruptPin 2 //Pin we are going to use to wake up the Arduino

const int chipSelect = 6;
char charFileName[16] = "";
String logString = "";
String fileName;
String tempVar;
File logFile;
float temperature_reading;
String temperature_str;
String logTime;
const int time_interval = 5;// Sets the wakeup intervall in minutes

void setup()
{
    Serial.begin(9600);        // Starts serial communication in 9600 baud rate.
    
    setSyncProvider(RTC.get);  // Library function to get the time from the RTC module.
    if(timeStatus() != timeSet) 
        Serial.println("System Time Cannot be Set. Check Connections.");
    else
        Serial.println("System Time is Set.");      

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  pinMode(LED_BUILTIN,OUTPUT);//We use the led on pin 13 to indecate when Arduino is A sleep
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on

  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);
  
  time_t t; //create a temporary time variable so we can set the time and read the time from the RTC
  t=RTC.get();//Gets the current time of the RTC
  RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);// Setting alarm 1 to go off 5 minutes from now
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
  RTC.squareWave(SQWAVE_NONE);
  // enable interrupt output for Alarm 1
  RTC.alarmInterrupt(ALARM_1, true);

}

void loop()
{
    displayTime();     // Prints the time to serial monitor.=
    fileName = getFileName();
//    tempVar = fileName;
    sprintf (charFileName, "%04d%02d%02d.log", year(), month(), day()); 
//    Serial.print ("----------- filename --->  ");
//    Serial.println( charFileName );
    temperature_reading = RTC.temperature() / 4.;
    temperature_str = F(",");
    temperature_str += String (temperature_reading, 4);
    logString = (displayTime());
    logString += temperature_str;
//    Serial.println (temperature_str);
//    Serial.println (displayTime());
    logFile = SD.open(charFileName, FILE_WRITE);
    if ( logFile ){
      logFile.println (logString);
      Serial.println (logString);
      logFile.close();
    }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file :- " + fileName);
  }
    
    delay(5000);       // 1 minute interval between prints.
    Going_To_Sleep();

}

String displayTime()  // Function that prints the time to serial monitor.
{
    logTime = year();
    logTime+= "-";
    logTime+= displayDigits(month(), '-')  + "-"
    + displayDigits(day(), '-') + " " 
    + displayDigits(hour(), ' ')  + ":"
    + displayDigits(minute(), ':')  + ":"
    + displayDigits(second(), ':');
//    Serial.println(); 

//    float c = RTC.temperature() / 4.;
//    Serial.println( c );
  return logTime;
}

String displayDigits(int digits, char delim)    // Function that prints preceding colon and leading 0.
{
    String digit;
    if(digits < 10){ 
        digit = "0" + String (digits);
    }
    else{
      digit = String (digits);
    }
    return digit;
}

String getFileName()  // Function get  filename for today.
{
    String filename = String( year() );
    if ( month() < 10 ){
      filename += "-0";
    }
    else{
      filename += "-";
    }
    filename += String( month() ) ;
    if ( day() < 10 ){
      filename += "-0";
    }
    else{
      filename += "-";
    }
    filename +=  String( day() ) ; //+ ".log"; 
//    Serial.println (hour());
//    Serial.println (minute());
//    Serial.println (second());
    return filename;
}

void Going_To_Sleep(){
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN,LOW);//turning LED off
    time_t t;// creates temp time variable
    t=RTC.get(); //gets current time from rtc
    Serial.println("Sleep  Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//prints time stamp on serial monitor
    delay(1000); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
    Serial.println("just woke up!");//next line of code executed after the interrupt 
    digitalWrite(LED_BUILTIN,HIGH);//turning LED on
//    temp_Humi();//function that reads the temp and the humidity
    t=RTC.get();
    Serial.println("WakeUp Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//Prints time stamp 
    //Set New Alarm
    RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);
  
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  }

void wakeUp(){
  Serial.println("Interrrupt Fired");//Print message to serial monitor
   sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
 
}
