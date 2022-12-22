#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Ultrasonic.h>

SoftwareSerial mySerial(11,12); // RX, TX

Ultrasonic ultrasonic1(2, 3);
Ultrasonic ultrasonic2(4, 5);
Ultrasonic ultrasonic3(6, 7);

const int buttonPin = 13;
int buttonState = 0;

int distance1;
int distance2;
int distance3;

static const int RXPin = 9, TXPin = 8;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
int count=0;
int buzzer=10;

SoftwareSerial ss(RXPin, TXPin);
double x;
double y;

void setup()
{
    Serial.begin(19200);
   // ss.begin(GPSBaud);
   // Serial.println(F("Testing GPS module"));
   // Serial.println();
    pinMode(buttonPin, INPUT);
    pinMode(A0, OUTPUT);      //Vibrator 1
    pinMode(A1, OUTPUT);      //Vibrator 2
    pinMode(A2, OUTPUT);      //Vibrator 3
    pinMode(buzzer, OUTPUT);      //Buzzer
}

void loop() // run over and over
{

  ///////// Water Level Sensor ////////////
  
  int sensorValue = analogRead(A3);
  Serial.println(sensorValue);
  digitalWrite(buzzer, LOW);
  if(sensorValue<400)
  {
    Serial.println("Water Detected");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
  }

    distance1 = ultrasonic1.read();
    distance2 = ultrasonic2.read();
    distance3 = ultrasonic3.read();
    
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW)
  {
      ss.begin(GPSBaud);
      Serial.println(F("Testing GPS module"));      
      Serial.println();
      mySerial.end();
  } 
      while (ss.available() > 0)
        if (gps.encode(ss.read()))
        {
          Serial.print(F("Location: ")); 
          if (gps.location.isValid())
          {
            x =  (gps.location.lat());
            Serial.print(x);
            Serial.print(F(","));
            y =  (gps.location.lng());
            Serial.print(y);
            Serial.println();
          }
          else
          {
            Serial.print(F("INVALID"));
          }
        }
     
  if (buttonState == HIGH)
  { 
            //Calling//

            ss.end();
            Serial.println("Calling/ Messaging through GSM Modem");
            mySerial.begin(19200);
            delay(2000);
            mySerial.println("ATD+917506953003;");
            delay(2000);
      
            //Messaging//
            
            mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
            delay(1000);  
            mySerial.println("AT+CMGS=\"+917506953003\"\r");
            delay(1000);
            mySerial.println("EMERGENCY");
            delay(100);
            mySerial.println("Latitude : ");
            mySerial.println(x);
            mySerial.println("Longitude : ");
            mySerial.println(y);
            mySerial.println((char)26);// ASCII code of CTRL+Z
            delay(100);
            delay(1000);
  }    
  if (mySerial.available())
  Serial.write(mySerial.read()); 

  /////// Ultrasonic code //////////////
  
  if(distance1 >= 10 && distance1 <= 20)  // FAR-LEFT
  {
      farleft();
  }
  if(distance2 >= 10 && distance2 <= 20)  // FAR-FRONT
  {
     farfront();
  }
  if(distance3 >= 10 && distance3 <= 20)  // FAR-RIGHT
  {
     farright();
  }
  
  if(distance1 <= 10) // NEAR-LEFT
  {
     nearleft();
  }
  if(distance2 <= 10) // NEAR-FRONT
  {
     nearfront();
  }
  if(distance3 <= 10) // NEAR-RIGHT
  {
     nearright();
  }
  
} 

/////////////   GPS Function   /////////////////

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
}


//////////// Ultrasonic Functions /////////////

void farleft()
{
  digitalWrite(A0, HIGH);   //Vibrator on
  delay(100);
  digitalWrite(A0,LOW);     //Vibrator off
  delay(50);
}

void farfront()
{
  digitalWrite(A1, HIGH);   //Vibrator on
  delay(100);
  digitalWrite(A1,LOW);     //Vibrator off
  delay(100);
}

void farright()
{
  digitalWrite(A2, HIGH);   //Vibrator on
  delay(100);
  digitalWrite(A2,LOW);     //Vibrator off
  delay(100);
}

void nearleft()
{
  digitalWrite(A0, HIGH);     //Vibrator on
  digitalWrite(buzzer,HIGH);  //Buzzer on
  delay(100);
  digitalWrite(A0,LOW);       //Vibrator off
  digitalWrite(buzzer,LOW);   //Buzzer off
  delay(100);
}

void nearfront()
{
  digitalWrite(A1, HIGH);       //Vibrator on
  digitalWrite(buzzer,HIGH);    //Buzzer on
  delay(100);
  digitalWrite(A1,LOW);         //Vibrator off
  digitalWrite(buzzer,LOW);     //Buzzer off
  delay(100);
}

void nearright()
{
  digitalWrite(A2, HIGH);       //Vibrator on
  digitalWrite(buzzer,HIGH);    //Buzzer on
  delay(100);
  digitalWrite(A2,LOW);         //Vibrator off
  digitalWrite(buzzer,LOW);     //Buzzer off
  delay(100);
}
