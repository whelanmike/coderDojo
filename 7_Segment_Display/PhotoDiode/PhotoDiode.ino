int lightSense = 0;

void setup()
{
    Serial.begin(9600);//Set Baud Rate to 9600 bps
}
 void loop()
{   unsigned int val;
    val=analogRead(lightSense);//Connect photodiode
    val=(5.0 * val) /1024;;
    if (val   < 1.00) {
    Serial.println(val);
//    Serial.println("V");
    }
    delay(50);
}
