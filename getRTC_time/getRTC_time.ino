#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // http://arduiniana.org/libraries/streaming/
#include <SPI.h>
#include <SD.h>

// Connect RTC SDA to Arduino pin A4.		DONE
// Connect RTC SCL to Arduino pin A5.		DONE
// Connect RTC INT/SQW to Arduino pin 2.

const uint8_t SQW_PIN(2);   // connect this pin to DS3231 INT/SQW pin

const int chipSelect = 6;
char charFileName[16] = "";
String logString = "";
String fileName;
String tempVar;
File logFile;
float temperature_reading;
String temperature_str;
String logTime;
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
	    
		RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
		RTC.alarm(ALARM_1);
		RTC.alarmInterrupt(ALARM_1, false);
		RTC.squareWave(SQWAVE_NONE);
		// configure an interrupt on the falling edge from the SQW pin
		pinMode(SQW_PIN, INPUT_PULLUP);
		attachInterrupt(INT0, alarmIsr, FALLING);

		// set alarm 1 for 20 seconds after every minute
		RTC.setAlarm(ALM1_EVERY_MINUTE, 0, 0, 0, 5);  // every 5 mins
		RTC.alarm(ALARM_1);                   // ensure RTC interrupt flag is cleared
		RTC.alarmInterrupt(ALARM_1, true)
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
      logFile.println( logString );
//      println (currentTimeStamp);
      logFile.close();
    }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file :- " + fileName);
  }
    
    delay(60000);       // 1 second interval between prints.
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
