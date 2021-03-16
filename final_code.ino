// Firebase_auth = 5Vrs7MSiQCis8SH0yf20Z4pZd6yA4FgEVk2JTGFV
// Firebase_Host = https://iot-project-8a872.firebaseio.com/

#include <ESP8266WiFi.h>
#include <Servo.h>

#include <FirebaseArduino.h>

#define FIREBASE_HOST "iot-project-8a872.firebaseio.com"           // the project name address from firebase id
#define FIREBASE_AUTH "5Vrs7MSiQCis8SH0yf20Z4pZd6yA4FgEVk2JTGFV"            // the secret key generated from firebase

#define WIFI_SSID "Goenkas"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "password9955"                                    //password for Wifi

String Available = "";                                                  //availability string
String fireAvailable = "";

//LiquidCrystal_I2C lcd(0x27, 16, 2);     //i2c display address 27 and 16x2 lcd display

Servo myservo;                          //servo as gate
Servo myservos;                         //servo as gate

int Empty;                              //available space integer
int allspace = 10;
int countYes = 0;

int carEnter = D4;                      // entry sensor 
int carExited = D0;                     //exit sensor

//int TRIG = D7;                        //ultrasonic trig  pin
//int ECHO = D8;                        // ultrasonic echo pin

int ledenter = D7;                           // car entering signal
int ledexit = D3;                           // car exiting signal

int pos;
int pos1;

float duration, distance;     

void setup() {
  Serial.begin (9600);                // serial debugging
  
  //Wire.begin(4, 5);                   // i2c start
  
  myservo.attach(D6);                   // servo pin to D6
  myservos.attach(D5);                  // servo pin to D5

  //myservo.write(0);
  //myservos.write(0);
  
  //pinMode(TRIG, OUTPUT);                // trig pin as output 
  //pinMode(ECHO, INPUT);                 // echo pin as input
  
  pinMode(ledenter, OUTPUT);
  pinMode(ledexit, OUTPUT);
  
  pinMode(carExited, INPUT);            // ir as input
  pinMode(carEnter, INPUT);             // ir as input

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);              //try to connect with wifi
  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);                           // display ssid
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");                               // if not connected print this
    delay(500);
  }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                    //print local IP address
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);      // begin firebase authentication
  //Firebase.pushString("Hello", fireAva);
  
  //lcd.begin(16,2);                          //begin lcd
  //lcd.init();
  //lcd.home();
  //lcd.setCursor(0, 0);                      // 0th row and 0thh column
  //lcd.print("Smart Parking");
}

void loop() {
  myservos.write(0);
  myservo.write(0);
  
  //digitalWrite(ECHO, LOW);            // make echo pin low
  //digitalWrite(TRIG, LOW);            // make trig pin low
  //delayMicroseconds(2);
  //digitalWrite(TRIG, HIGH);           // make trig pin high
  //delayMicroseconds(10);
  //digitalWrite(TRIG, LOW);
  
  //duration = pulseIn(ECHO, HIGH);
  //distance = duration * 0.01723;      // take distance in cm

  //Serial.print("Centimeter: ");
  //Serial.println(distance);

  int carEntry = digitalRead(carEnter);     // read ir input
  if (carEntry == LOW) {                    // if low then count and send data
    countYes++;                             // increment count
    if (countYes <= 10) {
      Serial.println("Car Entered" );
      Serial.print("Total Cars = " );
      Serial.println(countYes);
      Serial.print("Spots Left = " );
      Serial.println(allspace - countYes);
      
      //lcd.setCursor(0, 1);
      //lcd.print("Car Entered");

      //myservos.write(90);                     // change servo position
      for (pos = 0; pos <= 90; pos += 1) {        // change servo position
        myservos.write(pos);                       
        delay(5);
      }
      digitalWrite(ledenter, HIGH);
      delay(2000);

      //myservos.write(0);                      // change servo position
      for (pos = 90; pos >= 0; pos -= 1) {        // change servo position
        myservos.write(pos);                       
        delay(5);
      }
      digitalWrite(ledenter, LOW);

      Empty = allspace - countYes;                    //calculate available data
      Available = String("Available= ") + String(Empty) + String("/") + String(allspace);       // convert the int to string
      fireAvailable = String("Available=") + String(Empty) + String("/") + String(allspace);
      Firebase.pushString("/Parking Status/", fireAvailable );    // send string to firebase
      //lcd.clear();
    }
    else {
      Serial.print("No spots left!!" );
      //lcd.setCursor(0, 1);
      //lcd.print("Spots Full");
      delay(2000);
    }
  }

  int carExit = digitalRead(carExited);              //read exit ir sensor
  if (carExit == HIGH) {                             //if high then count and send
    countYes--;                                      //decrement count
    Serial.println("Car Exited" );
    Serial.print("Total Cars = " );
    Serial.println(countYes);
    Serial.print("Spots Left = " );
    Serial.println(allspace - countYes);
    
    //lcd.setCursor(0, 1);
    //lcd.print("Car Exited");

    //myservo.write(90);                     // change servo position
    for (pos = 0; pos <= 90; pos += 1) {        // change servo position
      myservo.write(pos);                       
      delay(5);
    }
    digitalWrite(ledexit, HIGH);
    delay(2000);

    //myservo.write(0);                      // change servo position
    for (pos = 90; pos >= 0; pos -= 1) {        // change servo position
      myservo.write(pos);                       
      delay(5);
    }
    digitalWrite(ledexit, LOW);
    
    Empty = allspace - countYes;                    //calculate available data
 
    Available = String("Available= ") + String(Empty) + String("/") + String(allspace);       // convert the int to string
    fireAvailable = String("Available=") + String(Empty) + String("/") + String(allspace);
    Firebase.pushString("/Parking Status/", fireAvailable );  // send string to firebase
    //lcd.clear();
  }
  
  //lcd.setCursor(0, 0);
  //lcd.print(Available);                     //print available data to lcd

}
