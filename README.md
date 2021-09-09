# Smarthome System Prototype

Created for my Master's thesis, "Internet of Things: The Next Generation". Developed in Arduino ecosystem, and has features of (1) carbon monoxide detection and (2) smart room light management. 

Features are implemented trough four modules: 
- System Management Module  
- Smoke Monitoring Module  
- Movement Monitoring Module  
- Actuator Management Module  

Conceptual model of system's module relations, and detail explanation of each module are presented below.

[konceptualni model sustava]

## System Management Module
Module consist of (1) ThingSpeak services which implement data layer of system, (2) Google Assistant voice interface for communication with user, and (3) IFTTT applets which send HTTP requests to data layer (ThingSpeak services) and to edge modules. Each of listed components is further explained below. 
### ThingSpeak
ThingSpeak services are composed of four data channels and services for CRUD operations on those data, which are used to support work of other parts of the system: 
#### Smoke Sensor Status 
The channel encapsulates configuration data (activity and maximum accepted level of carbon monoxide) about Smoke Monitoring Module. Its example is available on this URL: 
https://api.thingspeak.com/channels/1189072/feeds.json?api_key=31AIFTXDB7583BP0
#### Air Quality Log 
The channel encapsulates logs about carbon monoxide level in the measured room, collected in real time trough Smoke Monitoring Module. Its example is available on this URL:
https://api.thingspeak.com/channels/1199842/feeds.json?api_key=OWAJG6IJS9ZVTXO2
#### Room Light Status 
The channel encapsulates configuration data should the light in particular room be turned on or off. Its example is available on this URL: 
https://api.thingspeak.com/channels/1199845/feeds.json?api_key=1F4CJVZV752JQG39
#### Room Occupancy 
The channel encapsulates data about the presence of someone in the room, or in other words, is the room empty or occupied. Its example is available on this URL: 
https://api.thingspeak.com/channels/1199848/feeds.json?api_key=MAJLJ1EOJJ1BLP5F

### Google Assistant
Google Assistant presents an interface through which the user gives voice commands to the system. Thus, the user can communicate with the system from any location, only having to have his smartphone or another appropriate device. 

### IFTTT 
IFTTT applets are used to transpose user voice commands given to Google Assistant, to corresponding HTTP requests sent to ThingSpeak services. System is using three IFTTT applets: 
#### Room Light Management 
The applet on the basis of user voice command sets room light in the active or inactive status. A screenshot of applet specification is presented below. 

![image](https://user-images.githubusercontent.com/26325720/132256929-25ab432f-7f26-477f-a3bf-d408b9e1bd6b.png)

#### Gas Sensor Status 
The applet on the basis of user voice command set the Smoke Monitoring Module in the active or inactive status. A screenshot of applet specification is presented below. 

![image](https://user-images.githubusercontent.com/26325720/132256955-4c5a2a8d-0b04-4750-b898-75c596fa2f8a.png)

#### Maximum Acceptable CO Level
The applet on the basis of user voice command set the maximum acceptable level of carbon monoxide in the monitored room air. Value is measured through (carbon monoxide) parts per million parts of air. A screenshot of applet specification is presented below. 

![image](https://user-images.githubusercontent.com/26325720/132256980-47e5e0ea-cd49-42e8-b310-7c1d3d5e9adb.png)

## Smoke Monitoring Module
The module collects data on carbon monoxide level in the air, and sends that data to related ThingSpeak channel. Physically, the module is composed of: 
- ESP32 microcontroller (which has the Wi-Fi submodule) 
- MQ - 7 carbon monoxide sensor 
- Breadboard 
- Set of wires

Source code of the module's microcontroller is located and presented in folder [/smoke_monitoring_module](https://github.com/lovelmimica/SmarthomeSystemPrototype/blob/main/smoke_monitoring_module/smoke_monitoring_module.ino), and its photo is presented below.

![image](https://user-images.githubusercontent.com/26325720/132256552-4d288928-11a3-4970-8196-42eaddbd1149.png)

## Movement Monitoring Module
The module monitors the presence of movement in the monitored room, which indicates is the room occupied or empty, which is then sent to the related ThingSpeak channel. Physically, the module is composed of: 
- ESP32 microcontroller (which has the Wi-Fi submodule)
- Three PIR movement sensors
- Breadboard
- Set of wires

Source code of the module's microcontroller is located and presented in folder [/movement_monitoring_module](https://github.com/lovelmimica/SmarthomeSystemPrototype/blob/main/movement_monitoring_module/movement_monitoring_module.ino), and its photo is presented below. 

![image](https://user-images.githubusercontent.com/26325720/132256518-51073019-cd0b-45e6-8807-afcd266c2f09.png)

## Actuator Management Module
The module encapsulates the system's actuators (smoke alarm and bulb light submodule), and is performing tasks of turning them on and off. Physically, the module is composed of: 
- ESP32 microcontroller (which has the Wi-Fi submodule)
- Smoke alarm speaker
- Smoke alarm LED lamp
- 4-channel Arduino relay 
- 4.5V bulb light battery
- Bulb light
- Breadboard
- Set of wires

Source code of the module's microcontroller is located and presented in folder [/actuator_management_module](https://github.com/lovelmimica/SmarthomeSystemPrototype/blob/main/actuator_management_module/actuator_management_module.ino), and its photo is presented below.

![image](https://user-images.githubusercontent.com/26325720/132256471-6a2199a1-cd3f-4e3f-b6c2-b816975126f9.png)
