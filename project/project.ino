

#include <ESP8266WiFi.h>  // WIFI
#include <Firebase_ESP_Client.h>  // Firebase
#include <addons/RTDBHelper.h>  // Firebase
#include "DHT.h"  //DHT
#include <Wire.h> //LCD
#include <LiquidCrystal_I2C.h>  //LCD
#include <Servo.h>  // 서보

#define WIFI_SSID "yangminseok"    
#define WIFI_PASSWORD "04160416" 
#define DATABASE_URL "https://esp8266firebase-a017f-default-rtdb.asia-southeast1.firebasedatabase.app/" // RTDB의 URL
#define DATABASE_SECRET "ztBl1PdwsIMoF2dlIQ8fmlb6yHtu3QWX7wdzXhNf"    // RTDB의 비밀번호
#define BRIGHT_PIN A0
#define DHT_PIN D7
#define DHTTYPE DHT11
#define LED_PIN D10
#define SERVO_PIN D8

Servo myservo;
DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int readLEDstate = LOW;
int readServostate = 0;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long dataMillis = 0;

void setup(){
  Serial.begin(9600);
  dht.begin();
  lcd.init(); 
  lcd.backlight();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  myservo.attach(SERVO_PIN);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
}

void loop(){

  lcd.clear(); // 화면을 지워줍니다.
  lcd.setCursor(0, 0); // 첫 번째 줄로 커서 이동
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print("C");
  lcd.setCursor(0, 1); // 두 번째 줄로 커서 이동
  lcd.print("Humidity: ");
  lcd.print(dht.readHumidity());
  lcd.print("%");

  if (millis() - dataMillis > 2000) {
    dataMillis = millis();

    if(Firebase.RTDB.setInt(&fbdo, "/bright", analogRead(A0))){
      delay(5000);

      if( Firebase.RTDB.getInt(&fbdo, "/bright"))
      {
        String temp = fbdo.to<const char *>();                
        int brightness = temp.toInt();                            
        Serial.print("brightness : "); 
        Serial.println(brightness);    
      } else Serial.println(fbdo.errorReason().c_str());      
    }
    if(Firebase.RTDB.setInt(&fbdo, "/humidity", dht.readHumidity())){
      delay(5000);

      if( Firebase.RTDB.getInt(&fbdo, "/humidity"))
      {
        String temp = fbdo.to<const char *>();                
        int humidity = temp.toInt();                            
        Serial.print("humidity : "); 
        Serial.println(humidity);    
      } else Serial.println(fbdo.errorReason().c_str());
    }
    if(Firebase.RTDB.setInt(&fbdo, "/temperature", dht.readTemperature())){
      delay(5000);

      if( Firebase.RTDB.getInt(&fbdo, "/temperature"))
      {
        String temp = fbdo.to<const char *>();                
        int temperature = temp.toInt();                            
        Serial.print("temperature : "); 
        Serial.println(temperature);    
      } else Serial.println(fbdo.errorReason().c_str());
    }
    if(Firebase.RTDB.getInt(&fbdo, "/ledcontrol/ledauto")){
      String temp = fbdo.to<const char *>();
      int ledauto = temp.toInt();
      if(ledauto == 1){
        if( dht.readHumidity() > 50)
        {
          Firebase.RTDB.setInt(&fbdo, "/ledcontrol/ledautoval", 0);
        } else Firebase.RTDB.setInt(&fbdo, "/ledcontrol/ledautoval", 1);
        
        if( Firebase.RTDB.getInt(&fbdo, "/ledcontrol/ledautoval")) {   
          String temp = fbdo.to<const char *>();                
          readLEDstate = temp.toInt();                            
          Serial.print("LED state: "); 
          Serial.println(readLEDstate);  
        } else Serial.println(fbdo.errorReason().c_str());

        digitalWrite(LED_PIN, readLEDstate);
        delay(5000);
      }
      else{
        if( Firebase.RTDB.getInt(&fbdo, "/ledcontrol/ledsetval")) {   
          String temp = fbdo.to<const char *>();                
          readLEDstate = temp.toInt();                            
          Serial.print("LED state: "); 
          Serial.println(readLEDstate);  
        } else Serial.println(fbdo.errorReason().c_str());

        digitalWrite(LED_PIN, readLEDstate);
        delay(5000);
      }
    }    
    if(Firebase.RTDB.getInt(&fbdo, "/servocontrol/servoauto")){
      String temp = fbdo.to<const char *>();
      int ledauto = temp.toInt();
      if(ledauto == 1){
        if( dht.readTemperature() > 26 && analogRead(A0) > 900)
        {
          Firebase.RTDB.setInt(&fbdo, "/servocontrol/servoautoval", 0);
        } else Firebase.RTDB.setInt(&fbdo, "/servocontrol/servoautoval", 90);
          
        if( Firebase.RTDB.getInt(&fbdo, "/servocontrol/servoautoval")) {   
          String temp = fbdo.to<const char *>();                
          readServostate = temp.toInt();                            
          Serial.print("Servo state: "); 
          Serial.println(readServostate);  
        } else Serial.println(fbdo.errorReason().c_str());

        myservo.write(readServostate);
        delay(5000);
      }
      else{
        if( Firebase.RTDB.getInt(&fbdo, "/servocontrol/servosetval")) {   
          String temp = fbdo.to<const char *>();                
          readServostate = temp.toInt();                            
          Serial.print("SERVO state: "); 
          Serial.println(readServostate);  
        } else Serial.println(fbdo.errorReason().c_str());

        myservo.write(readServostate);
        delay(5000);        
      }
    }
  }
}
