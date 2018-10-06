/*
 * ----------------------------------------------------------------------------
 * This sketch uses the MFRC522 library ; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * This sketch show a simple locking mechanism using the RC522 RFID module.
 * ----------------------------------------------------------------------------
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

 /*
  LiquidCrystal Library 

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 10
 * LCD D5 pin to digital pin 9
 * LCD D6 pin to digital pin 8
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

 // -------------------------------------- Differnet Header Files--------------------------------------//
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h> 



//--------------- Initialization of Different Vaiables/Constants for Arduino Pins----------------------//

#define RST_PIN         5           // Configurable, see typical pin layout above for RFID Module
#define SS_PIN          53          // Configurable, see typical pin layout above for RFID Module

int redPin= 9;                      // Pin For RGB LED (Red)
int greenPin = 10;                  // Pin For RGB LED (Green)
int bluePin = 11;                   // Pin For RGB LED (Blue)
int servoPin =22;                   // Pin For Servo Motor
int pos = 70;                       // Starting Position of the servo motor
int buzzer = 13;                    // Pin For Buzzer 
 
 

//----------------------------------------------  Instances --------------------------------------------//

MFRC522 mfrc522(SS_PIN, RST_PIN);   
MFRC522::MIFARE_Key key;
LiquidCrystal lcd(24, 25, 26, 27, 28, 29);
Servo myservo;


//---------------------------------------- Variable For User Information and Card ID ---------------------//
String read_rfid;
String ok_rfid_1="393b92ab";
String ok_rfid_2="b688a45"; 
String rfid1_user="Ashfaq";
String rfid2_user="Abid";
int rfid1_balance=5000;
int rfid2_balance=200;



//---------------------------------------------  SETUP  -----------------------------------------------------//

void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card

    
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    pinMode(buzzer, OUTPUT); 
    
    myservo.attach(servoPin);  

      lcd.begin(16, 2);
      lcd.setCursor(0,0);
      lcd.clear();

   
}


//-------------------- Helper routine to dump a byte array as hex values to Serial-------------------//



void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}




//----------------------------------------- For Opeing the Gate Of The Toll System --------------------------//

void open_lock() {
 
      setColor(255, 255, 255); // NO Color
      setColor(255, 0, 255); // Green Color
      delay(3000);
      setColor(255, 255, 255); // NO Color
  
 
        for (pos = 70; pos <= 160; pos=pos+1)
        {
          myservo.write(pos);              
          delay(20);                       
        }
      
        delay(3000); 
        
        for (pos = 160; pos >= 70; pos=pos-1) 
        { 
          myservo.write(pos);              
          delay(20);                       
        }
  
}



//--------------------------------------------For Locking The Gate of The Toll System ------------------------//

void error_lock()
{
       
        tone(buzzer, 5000);
        setColor(255, 255, 255); 
        setColor(0, 255, 255); // RED Color
        delay(3000);
        setColor(255, 255, 255); // NO Color
        noTone(buzzer);

      
  
}


//--------------------------------------------------For Selecting The Color of the LED -------------------------------//

 void setColor(int redValue, int greenValue, int blueValue) {
      analogWrite(redPin, redValue);
      analogWrite(greenPin, greenValue);
      analogWrite(bluePin, blueValue);
    }




//--------------------------------------------------LOOP FUNCTION---------------------------------------------------//
    
void loop() {
   setColor(255, 255, 255); // NO Color
   lcd.setCursor(0, 0);

      // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    //------------------------------------ TO READ THE UID OF THE RFID KEY -------------------------------//
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

    //------------------------------------- TO CHECK IN SERIAL MONITOR ----------------------------------//
    Serial.println("User ID: ");
    Serial.println(read_rfid);
    Serial.println('\n');


    //----------------------------------------- For Valid User ----------------------------------------//
    if (read_rfid==ok_rfid_1||read_rfid==ok_rfid_2) {

   
        if(rfid1_balance>=50 && read_rfid==ok_rfid_1 )
        { 
              setColor(255, 255, 255); // NO Color
              setColor(255, 0, 255); // Green Color
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("User Name:");
              lcd.setCursor(0,1);
              lcd.print(rfid1_user);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("User ID:");
              lcd.setCursor(0,1);
              lcd.print(read_rfid);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Current Balance:");
              lcd.setCursor(0,1);
              lcd.print(rfid1_balance);
              delay(2000);
              rfid1_balance=rfid1_balance-50;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Balance After:");
              lcd.setCursor(0,1);
              lcd.print(rfid1_balance);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Thank You!!!");
              delay(2000);
    
              open_lock();
              lcd.clear();
        }

        
        else if(rfid2_balance>=50 && read_rfid==ok_rfid_2)
        {
              setColor(255, 255, 255); // NO Color
              setColor(255, 0, 255); // Green Color
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("User Name:");
              lcd.setCursor(0,1);
              lcd.print(rfid2_user);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("User ID:");
              lcd.setCursor(0,1);
              lcd.print(read_rfid);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Current Balance:");
              lcd.setCursor(0,1);
              lcd.print(rfid2_balance);
              delay(2000);
              rfid2_balance=rfid2_balance-50;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Balance After:");
              lcd.setCursor(0,1);
              lcd.print(rfid2_balance);
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Thank You!!!");
              delay(2000);
    
              open_lock();
              lcd.clear();

          
         }

         
        else 
        {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Insufficient");
              lcd.setCursor(0, 1);
              lcd.print("Balance");
              error_lock();
              lcd.clear();
           
         }  
           
    }


    //---------------------------------------- For Invalid User ---------------------------------------//
    
    else 
    {
      setColor(255, 255, 255); // NO Color
      setColor(0, 255, 255); // Green Color
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(read_rfid);
      lcd.setCursor(0, 1);
      lcd.print("Invalid User");
      
      error_lock(); 
         
    }
     
         
}



