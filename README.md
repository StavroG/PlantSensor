# PlantSensor
### A tool that measure the soil moisture, temperature, and lux value of a plants environment and stores that information in a Firebase realtime database.

**What I Learned:**
- How to use Firebase to store data in real time
- How to use a NodeMCU devboard connected to WiFi to send data to a Firebase real time database
- How to read sensor values from photoresistors, thermistors and capacitive soil sensor

**Materials:**
- ESP8266 NodeMCU
- Capacitive soil moisture sensor
- Photoresistor
- Thermistor
- 1N4007 Diode x 3 (Not necessary if you are using a dev board with multiple analog input pins)
- 10K Ohm resitor x 2

**Note:**
This project uses a NodeMCU and some sensors to get the soil moisture, temperature and lux values of a plants environment. This is then saved in a Firebase database and can be used in a mobile app, web app, etc. This project utilizes a NodeMCU because of the need to send data over WiFi. I recommend using the same board but if you wish to use another board that is fine, but you will need a way to connect to WiFi as well as a stable WiFi connection. In the materials list I did not provide any materials about powering the device. Depending on what device you are using you will have to satisfy different power requirements. 









