float temperature;
int tempPin = 0;

void setup()
{
    Serial.begin(9600);//Set Baud Rate to 9600 bps
}
 void loop()
{   unsigned int val;
    unsigned int dat;
    val=analogRead(tempPin);//Connect LM35 on Analog 0
    temperature=(500 * val) /1024;;
    Serial.print("Temp:"); //Display the temperature on Serial monitor
    Serial.print(temperature);
    Serial.println("C");
    delay(5000);
}
