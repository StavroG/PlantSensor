/*
 * Author: Stavro Gorou 
 * GitHub: https://github.com/StavroG
 * Date: 08/14/21
 * Materials: ESP8266 NodeMCU, Capacitive Soil Moisture Sensor, Photoresistor, Termistor, 1N4007 Diode x 3, 10k Ohm Resistor x 2
 * Description: A tool that measure the soil moisture, temperature, and lux value of a plants environment and stores that information in a Firebase realtime database.
 */

/*
 * Required Libraries:  
 * ArduinoJson by BEnoit Blanchon Verison 5.13.5 from Library Manager
 * GyverNTC by Alex Gyver Version 1.0.0 from Library Manager
 * Firebase Arduino from https://github.com/FirebaseExtended/firebase-arduino/tree/master/src
 * If using a NodeMCU add http://arduino.esp8266.com/stable/package_esp8266com_index.json to 
 * File > Preferences > Additional Boards Managers URLs, if you have multiple add comma between each link
*/

/* 
 *  Firebase uses a fingerprint for security. This fingerprint is updated every year. 
 *  50 89 50 57 90 1F 37 E3 B8 F3 5B 02 ED 3A 65 6E 6A 34 DB 93 is the finger print as of August 2021.
 *  If this fingerprint is no longer valid you can get the new fingerprint by going to https://www.grc.com/fingerprints.htm and entering 
 *  test.firebaseio.com in the search bar.
 *  You should be given a new fingerprint that looks like 50:89:50:57:90:1F:37:E3:B8:F3:5B:02:ED:3A:65:6E:6A:34:DB:93.
 *  To change this fingerprint in the library go to where you saved the firebase library then firebase-arduino (Or whatever you saved it as) > src > FirebaseHttpClient.h
 *  Once you are in this file scroll to the bottom and change the firebase fingerprint.
 *  Replace all the : with empty space and save, it should look like this 50 89 50 57 90 1F 37 E3 B8 F3 5B 02 ED 3A 65 6E 6A 34 DB 93.
 */
 
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h> 
#include <GyverNTC.h>

#define WIFI_SSID "WiFi SSID" //WiFi SSID
#define WIFI_PASS "WiFi Password"  //WiFi password
#define FIREBASE_HOST "Firebase Host" //Get host from firebase realtime database, delete https:// and the / after .com
#define FIREBASE_AUTH "Firebase Authentication "  //Get auth from: project settings > service account > Database secrets

const int analog = A0;  //Analog pin on NodeMCU
const int moistureSensor = D0;  //Set moisture sensor power in to digital pin 0
const int temperatureSensor = D2; //Set temperature sensor power in to digital pin 2
const int luxSensor = D4; //Set lux sensor power in to digital pin 4

GyverNTC therm(0, 10000, 10000);  //Using photoresistor rated at 10k Ohms with a 10k Ohm resistor

void setup() {
  Serial.begin(9600); //Open serial communication with baud rate of 9600
  
  WiFi.begin(WIFI_SSID, WIFI_PASS); //Attemts to connect to WiFi
  while(WiFi.status() != WL_CONNECTED) {  //If can not connect to WiFi display message
    Serial.println("Please connect to WiFi ...");
    delay(500);
  }
  Serial.print("Connected to: "); //If successfully connected display local ip
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(moistureSensor, OUTPUT);  //Set pinmode of the digital pin connected to the soil moisture sensor to OUTPUT
  pinMode(temperatureSensor, OUTPUT);//Set pinmode of the digital pin connected to the temperature sensor to OUTPUT
  pinMode(luxSensor, OUTPUT);//Set pinmode of the digital pin connected to the lux sensor to OUTPUT
  
  digitalWrite(moistureSensor, LOW);  //Start with all pins turned to LOW
  digitalWrite(temperatureSensor, LOW);
  digitalWrite(luxSensor, LOW);
  
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Connect to Firebase with credentials 
}

void loop() {
  int moisture = getMoistureValue();  //Gets the soil moisture sensor's value
  float temp = getTemperatureValue(); //Gets the temperature sensor's value
  int lux = getLuxValue();  //Gets the lux sensor's value

  Firebase.setInt("Moisture", moisture);  //Update firebase with soil moisture sensor's reading
  if (Firebase.failed()) {  //Display error message if could not send data
    Serial.print("Error setting moisture percentage:");
    Serial.println(Firebase.error());
    return;
  }
  Firebase.setFloat("Temperature", temp); //Update firebase with temperature sensor's reading
  if (Firebase.failed()) {  //Display error message if could not send data
    Serial.print("Error setting temperature:");
    Serial.println(Firebase.error());
    return;
  }
  Firebase.setInt("Lux", lux);  //Update firebase with lux sensor's reading
  if (Firebase.failed()) {  //Display error message if could not send data
    Serial.print("Error setting lux value:");
    Serial.println(Firebase.error());
    return;
  }

  Serial.println();
  delay(60000); //Run every 60 seconds
}

int getMoistureValue() {
  digitalWrite(temperatureSensor, LOW);  //Turn all pins other than soil moisture sensor's pin off
  digitalWrite(luxSensor, LOW);
  digitalWrite(moistureSensor, HIGH);

  delay(1500); //Wait 1.5 seconds to make sure all other pins turned off
  
  int sensorValue = analogRead(analog); //Reads the value of the soil moisture sensor
  int sensorMax = 705;  //Max value from the sensor, this will be 100% 
  int sensorMin = 330;  //Min value from the sensor, this will be the 0%
  int percentage = map(sensorValue, sensorMax, sensorMin, 0, 100);  //Turns the sensor value into a percentage using the max and min

  Serial.print("Soil Moisture Percentage: "); //Print soil moisture percentage
  Serial.println(percentage);

  return percentage;  //Return the soil moisture percentage
}

float getTemperatureValue() {
  digitalWrite(moistureSensor, LOW);  //Turn all pins other than temperature sensor's pin off
  digitalWrite(luxSensor, LOW);
  digitalWrite(temperatureSensor, HIGH);

  delay(1500);  //Wait 1.5 seconds to make sure all other pins turned off
  
  float temp = therm.getTempAverage();  //Gets the temperature from the temperature sensor
  
  Serial.print("Temperature: ");  //Print the temperature
  Serial.print(temp);
  Serial.println(" C");

  return temp;  //Return the temperature 
}

int getLuxValue() {
  digitalWrite(moistureSensor, LOW);  //Turn all pins other than lux sensor's pin off
  digitalWrite(temperatureSensor, LOW);
  digitalWrite(luxSensor, HIGH);

  delay(1500);  //Wait 1.5 seconds to make sure all other pins turned off

  int lux = analogRead(analog); //Read the lux value from the sensor
  
  Serial.print("Lux: ");  //Print the lux value
  Serial.println(lux);

  return lux; //Return the lux value
}
