char               *logString ;
char               charFileName[16]   = "";
float              temperature_float  = 0.0;
char               temperature_char[10];
unsigned int       Year, Month, Day, Hour, Minute, Second;  // Function to update these values.
unsigned long      currentTime        = 0;
unsigned long      lastCheckTime      = 0 ;
unsigned int       repeatCheckMinutes = 1 ;
unsigned int       currentPressure    = 0;
unsigned int       previousPressure   = 0;
const unsigned int pressureLow        = 10;
const unsigned int pressureHigh       = 300;

int                randMinute ;
int                randSecond ;

void setup()
{
//    Wire.begin();   // REM
    Serial.begin(9600);        // Starts serial communication in 9600 baud rate.
    Serial.println("System Ready.");
    Year = 2019;
    Month = 9;
    Day = 19;
    Hour = 16;
    Minute = 34;
    Second = 22;
}

void loop()
{
        logString = displayTime();
        Serial.println (logString);
        free (logString);
        randMinute = random (1, 8);
        randSecond = random (1, 9);
        
        Minute +=  randMinute;
        Minute = Minute % 60 ;

        Second += randSecond;
        Second = Second % 60 ;

        
//        logString += "|" + String(temperature_char) +"|";
//        logString += String (currentPressure) + "|";
delay (5000);
}

/* END loop . Define functions after this */

char * displayTime(){                     // return timestamp (YYYY-MM-DD HH:MM:SS) as character array.
    char * current_timestamp = (char *) malloc (21);;
    sprintf(current_timestamp, "%4d-%02d-%02d %02d:%02d:%02d", Year, Month, Day, Hour, Minute, Second);
    Serial.print (" >> In fn displayTime  : - ") ;
    Serial.println (current_timestamp);
    return current_timestamp;
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
