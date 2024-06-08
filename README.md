# BerdeBox

[Landing Page](https://berde-box.vercel.app/)
[Web App Page](https://berde-box.vercel.app/app)

![image](https://github.com/bbcarrots/BerdeBox/assets/56602966/c854eaa2-08eb-47d3-a304-ce42ce83f15a)

BerdeBox is a remote delivery pick-up box, which is fully controllable by your phone so that you can accept packages, anywhere, anytime. Aside from picking up delivieries, BerdeBox includes a cashbox module which allows users to complete Cash-on-Delivery transactions remotely. 

This is a project for UP Diliman's Department of Computer Science's IoT Cup 2024. 

## Prototype Features
BerdeBox is a smart mailbox that can function as a personal delivery assistant. The following features allow for convenience and assurance:
1. The application sends push notifications when the delivery person presses the doorbell, to notify the user that their package has arrived.
2. Photos are taken when the delivery person presses the doorbell, when the lockbox has been opened and when the cashbox module has been opened, to allow for verification. These photos can be accessed in the log.
3. An auto-locking mechanism has been implemented, to ensure that the mailbox will close and lock even if the delivery man fails to do so.


## Technology
### Hardware
An `ESP8266` was the main microcontroller of BerdeBox. Most of the sensors (doorbell button) and actuators (main lock servo, cashbox lock solenoid, 2 LEDs, buzzer) are connected to the ESP8266.
An `ESP32 CAM` is used for taking photos. The camera is connected to the `ESP32`. We decided to use the Firebase library for the `ESP8266` for direct communication with the Firebase Realtime Database (RDB). However, the same could not be done with the `ESP32` because it would need to have access to both the `RDB` and Firebase Storage. Hence, all communication with the RDB is done by the `ESP8266` and the `ESP32` only uploads a photo to the Firebase Storage. For the `ESP32` to take a photo when needed, the `ESP8266` has to send a packet using the `ESP NOW` library which enables communication between ESP devices through the WiFi.

Sensors and Actuators         |  Circuit Set-up 
:-------------------------:|:-------------------------:
![20240529_123317](https://github.com/bbcarrots/BerdeBox/assets/56602966/19f141f1-b42b-4675-94f5-9d68379674d2) | ![20240529_123326](https://github.com/bbcarrots/BerdeBox/assets/56602966/6c6fde9a-8105-448e-9ea6-ab7671c5ca61)


### Database
Since the project required data to be persistent, we decided to use Google Firebase to store all of our data. We used the following Firebase services
1. `Firebase Realtime Database` for communications between the web app and the hardware
2. `Firebase Storage` for uploading photos from the ESP32 CAM and retrieving photos to display in the web app.
3. `Firestore Database` to store user information and BerdeBox information
4. `Firebase Authentication`, Firebase Cloud Messaging and Firebase Cloud Functions for registering and implementing push notifications based on changes in the real-time database

### Software
We decided to use `SvelteKit` for our web app. To allow our web app to simulate our ideal final product, we turned it into a progressive web application to simulate a native mobile application and allow push notifications. 

Using Firebase, we were able to communicate with the changes from the hardware using Firebase Real-time Database. We were also able to retrieve the photos taken by the ESP32 camera by accessing Firebase Storage.

With Firebase, we are able to implement authentication to allow users to add their own BerdeBoxes and implement push notifications with Firebase Cloud Messaging and Firebase Cloud Functions. These were implemented to simulate the user flow when using the mobile application.

Home Page        |  Controls | Logs
:-------------------------:|:-------------------------:|:-------------------------:
![image](https://github.com/bbcarrots/BerdeBox/assets/56602966/cb719a67-ea0b-497c-9d55-97eaf04aa9e7) | ![image](https://github.com/bbcarrots/BerdeBox/assets/56602966/8db68b53-62c6-41df-b439-995dd96fcc66) | ![image](https://github.com/bbcarrots/BerdeBox/assets/56602966/8ac1e677-7e5e-45f2-b08f-31175ff741a1)


## Demo

1. Opening BerdeBox remotely
   
https://github.com/bbcarrots/BerdeBox/assets/56602966/6e78c98b-6ed2-4f48-88b5-e7811748dfc9

3. Opening the cashbox module remotely
   
https://github.com/bbcarrots/BerdeBox/assets/56602966/7a5ebc76-c62c-4d0a-99f7-1571599a6dc2
