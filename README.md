# Smarthome System Prototype

Smarthome System Prototype is simple Internet of Things system developed for purpose of learning associated technologies. It has features of (1) carbon monoxide level monitoring and (2) smart room light management, which are implemented trough four modules: 
1/ System Management Module
2/ Smoke Monitoring Module
3/ Movement Monitoring Module
4/ Actuator Management Module

Conceptual model of system's module relations, and detail explanation of each module are presented below.
[konceptualni model sustava]


1/ System Management Module
Module consist of (1) ThingSpeak services which implement data layer of system, (2) Google Assistant voice interface for communication with user, and (3) IFTTT applets which send HTTP requests to data layer (ThingSpeak services) and to edge modules. Each of listed components is further explained below. 
1/ ThingSpeak
ThingSpeak services are composed of four data channels and services for CRUD operations on those data, which are used to support work of other parts of the system: 
1/ Smoke Sensor Status 
The channel encapsulates configuration data (activity and maximum accepted level of carbon monoxide) about Smoke Monitoring Module. Its example is available on this URL: 
https://api.thingspeak.com/channels/1189072/feeds.json?api_key=31AIFTXDB7583BP0
2/ Air Quality Log 
The channel encapsulates logs about carbon monoxide level in the measured room, collected in real time trough Smoke Monitoring Module. Its example is available on this URL:
https://api.thingspeak.com/channels/1199842/feeds.json?api_key=OWAJG6IJS9ZVTXO2
3/ Room Light Status 
The channel encapsulates configuration data should the light in particular room be turned on or off. Its example is available on this URL: 
https://api.thingspeak.com/channels/1199845/feeds.json?api_key=1F4CJVZV752JQG39
4/ Room Occupancy 
The channel encapsulates data about the presence of someone in the room, or in other words, is the room empty or occupied. Its example is available on this URL: 
https://api.thingspeak.com/channels/1199848/feeds.json?api_key=MAJLJ1EOJJ1BLP5F

2/ Google Assistant
Google Assistant presents an interface through which the user gives voice commands to the system. Thus, the user can communicate with the system from any location, only having to have his smartphone or another appropriate device. 

3/ IFTTT 
IFTTT applets are used to transpose user voice commands given to Google Assistant, to corresponding HTTP requests sent to ThingSpeak services. System is using three IFTTT applets: 
1/ Room Light Management applet on the basis of user voice command sets room light in the active or inactive status. A screenshot of applet specification is presented below. 
[slika apleta]

2/ Gas Sensor Status applet on the basis of user voice command set the Smoke Monitoring Module in the active or inactive status. A screenshot of applet specification is presented below. 
[slika apleta]

3/ Maximum Acceptable CO Level applet on the basis of user voice command set the maximum acceptable level of carbon monoxide in the monitored room air. Value is measured through (carbon monoxide) parts per million parts of air. A screenshot of applet specification is presented below. 
[slika apleta]

2/ Smoke Monitoring Module
The module collects data on carbon monoxide level in the air, and sends that data to related ThingSpeak channel. Physically, the module is composed of: 
1/ ESP32 microcontroller (which has the Wi-Fi submodule)
2/ MQ - 7 carbon monoxide sensor
3/ Breadboard
4/ Set of wires
Source code of the module's microcontroller is located and presented in related folder (/smoke_monitoring_module), and its photo is presented below.
![image](https://user-images.githubusercontent.com/26325720/132256552-4d288928-11a3-4970-8196-42eaddbd1149.png)

3/ Movement Monitoring Module
The module monitors the presence of movement in the monitored room, which indicates is the room occupied or empty, which is then sent to the related ThingSpeak channel. Physically, the module is composed of: 
1/ ESP32 microcontroller (which has the Wi-Fi submodule)
2/ Three PIR movement sensors
3/ Breadboard
4/ Set of wires
Source code of the module's microcontroller is located and presented in related folder (/movement_monitoring_module), and its photo is presented below. 
![image](https://user-images.githubusercontent.com/26325720/132256518-51073019-cd0b-45e6-8807-afcd266c2f09.png)

4/ Actuator Management Module
The module encapsulates the system's actuators (smoke alarm and bulb light submodule), and is performing tasks of turning them on and off. Physically, the module is composed of: 
1/ ESP32 microcontroller (which has the Wi-Fi submodule)
2/ Smoke alarm speaker
3/ Smoke alarm LED lamp
4/ 4-channel Arduino relay 
5/ 4.5V bulb light battery
6/ Bulb light
7/ Breadboard
8/ Set of wires
Source code of the module's microcontroller is located and presented in related folder (/actuator_management_module), and its photo is presented below.
![image](https://user-images.githubusercontent.com/26325720/132256471-6a2199a1-cd3f-4e3f-b6c2-b816975126f9.png)
