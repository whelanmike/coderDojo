/*
 * 
 * All the resources for this project: https://randomnerdtutorials.com/
 * Modified by Rui Santos
 * 
 * Created by FILIPEFLOP
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>

int ENA = 5;
#define IN1 4
#define IN2 3
#define FAIL_LED 6
#define SUCCESS_LED 7

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  
 // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Move your card to the reader...");
  Serial.println();
  pinMode (FAIL_LED, OUTPUT);
  pinMode (SUCCESS_LED, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENA, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 170);

}
void loop() 
{
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "DA 78 E3 0A" or content.substring(1) == "XAB C7 00 1B" or content.substring(1) == "79 63 5E 3F") //change here the UID of the card/cards that you want to give access
    {
      Serial.println("Authorized access");
      Serial.println();
      if (content.substring(1) == "79 63 5E 3F"){
        Serial.println("   *** Visa Card *** ");
        }
      digitalWrite(SUCCESS_LED, HIGH);
      delay(3000);
    }

    else   {
      Serial.println(" Access denied");
      digitalWrite(FAIL_LED, HIGH);
      delay(3000);
    }
    digitalWrite(SUCCESS_LED, LOW);
    digitalWrite(FAIL_LED, LOW);
    Serial.println("End of prog");
  
} 