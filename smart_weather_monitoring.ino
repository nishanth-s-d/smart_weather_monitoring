#include <DHT.h>
#include <DHT_U.h>

#include <WiFi.h>                                                // esp32 library

#include <IOXhop_FirebaseESP32.h>                                         // firebase library

#define FIREBASE_HOST "nishanth-e18ed.firebaseio.com"                         // the project name address from firebase id

#define FIREBASE_AUTH "KVzQGhMePZyhpwE4kQqCJ05AXiF1YYxOxjou9R1q"                    // the secret key generated from firebase

#define WIFI_SSID "Raviteja_guna"                                          // input your home or public wifi name

#define WIFI_PASSWORD "drt_teja$"


//DHT
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Rain Sensor
int sensorPin = 33;
int sensorValue2 = 0; 

//Gas Sensor
#define MQ2pin 35
float sensorValue3; 

void setup() {
 
           //LDR
    Serial.begin(9600);
    pinMode(32, OUTPUT); 
  
  // put your setup code here, to run once:

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   
     
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //DHT
  dht.begin();
   Serial.println("");
    Serial.println("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST);

//Gas Sensor
  Serial.println("Gas sensor warming up!");  
}

void firebasereconnect(){
  Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }



void loop() {
  // put your main code here, to run repeatedly:
if (Firebase.failed()) {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());  
      firebasereconnect();
      return;
  }
  
//DHT
float h = dht.readHumidity();
float t = dht.readTemperature();
Firebase.setFloat ("Temp",t);
Firebase.setFloat ("Humidity",h);
Serial.print("Current temperature is: ");
Serial.println(t);
Serial.print("Humidity: ");
Serial.println(h);


//Rain Sensor
sensorValue2 = analogRead(sensorPin);

if (sensorValue2<4095)
{
  Serial.print("Rain is detected");
  }
  
  else
{
  Serial.print("Rain not detected");
  }

Serial.print("  Rain value: ");

Serial.println(sensorValue2);

Serial.println();
Firebase.set("Raindrop",sensorValue2);


//LDR
int sensorValue = analogRead(32);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (3.0 / 1023.0);
  // print out the value you read:
  
      Firebase.set("LightIntensity",voltage);
      Serial.print("Light Intensity is: ");
  Serial.println(voltage);


//Gas Sensor
  sensorValue3 = analogRead(MQ2pin); // read analog input pin 0  
  Serial.print("Smoke Sensor Value: ");
  Serial.print(sensorValue3);
  if(sensorValue3 > 300)
  {
    Serial.print(" | Smoke detected!");
  }
  
  Serial.println("");
   Firebase.set("GasSensor",sensorValue3);
delay(500);
}
