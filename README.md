#SmartSecure Living System
## Overview
The SmartSecure Living System is an IoT-based security and smart home system that integrates motion detection, alarm control, and lighting automation. The system uses Firebase Realtime Database to synchronize data between a web interface and an embedded microcontroller (ESP32 or ESP8266). The web interface allows users to monitor and control the alarm and light statuses in real-time, while the embedded system interacts with sensors and hardware to control physical devices.

## Features
- Motion Detection: The system monitors a sensor for motion. When motion is detected, the alarm and light are activated.
- Remote Control: Users can remotely control the alarm and light through the web interface.
- Notifications: The system provides notifications when motion is detected, including a timestamp.
- System State Control: The user can toggle the system's state (ON/OFF), which affects the status of the alarm and light controls.
- Real-Time Data Sync: The system synchronizes real-time data between the web interface and the microcontroller using Firebase Realtime Database.

## Components
- Web Interface: A responsive web page built with HTML, CSS, and JavaScript. It allows users to control the alarm and light, toggle notifications, and see real-time system updates.
- Microcontroller (ESP32/ESP8266): The embedded device that reads sensor data, controls the alarm and light, and communicates with Firebase.
- Firebase Realtime Database: Used to store and retrieve the state of the system, alarm, light, sensor, and notifications.

## Setup and Installation
- Setting up Firebase

	- Go to the Firebase Console.
	- Create a new project or use an existing one.
	- Add a new web app to your Firebase project and copy the Firebase configuration for your app.
	- Enable Firebase Realtime Database in the Firebase Console.

- Firebase Configuration

	- In your web app's JavaScript code (script2.js), replace the firebaseConfig object with your own Firebase configuration:

```javascript
const firebaseConfig = {
  apiKey: "YOUR_API_KEY",
  authDomain: "YOUR_PROJECT_ID.firebaseapp.com",
  databaseURL: "https://YOUR_PROJECT_ID.firebaseio.com",
  projectId: "YOUR_PROJECT_ID",
  storageBucket: "YOUR_PROJECT_ID.appspot.com",
  messagingSenderId: "YOUR_SENDER_ID",
  appId: "YOUR_APP_ID",
};
```

- Microcontroller Setup
	- Install the required libraries for Firebase and Wi-Fi in the Arduino IDE:
		- Firebase ESP Client
		- WiFi (for ESP32/ESP8266)
- Replace the following placeholder values in the arduino_code:
	- WIFI_SSID and WIFI_PASSWORD: Your Wi-Fi credentials.
	- API_KEY: Your Firebase project's API key.
	- DATABASE_URL: Your Firebase Realtime Database URL.

- Uploading the Code to the Microcontroller
	- Upload the Arduino sketch to your ESP32 or ESP8266 device. Ensure that the device is connected to your local Wi-Fi network and can communicate with Firebase.

- Web Interface Setup
	- Ensure your web page (index.html, style.css, script2.js) is properly set up and the Firebase configuration is correct. You can host this web interface locally or on a platform like GitHub Pages or Firebase Hosting.

## How It Works
- Motion Detection: The system uses a sensor (connected to the microcontroller) to detect motion. When motion is detected, the microcontroller updates the Firebase database with the status of the alarm and light.
- Alarm and Light Control: Users can toggle the alarm and light states using the web interface. This information is sent to Firebase, which updates the microcontroller accordingly.
- Notifications: When both the alarm and light are activated, the system will create a notification with the current date and time.
- System State: The user can toggle the overall system state (ON/OFF) via a checkbox. This controls whether the alarm and light buttons are enabled.
## Code Structure
#### Web Interface
- index.html: The main HTML file that contains the structure and layout of the web interface.
- style.css: The CSS file for styling the web page.
- script2.js: The JavaScript file that controls the Firebase connection and the dynamic interaction with the buttons and notifications.
#### Embedded System (Arduino Code)
- arduino_code: The Arduino sketch that runs on the ESP32 or ESP8266. It handles Wi-Fi connectivity, Firebase communication, sensor reading, and controlling the alarm and light.
## How to Use
- Open the web interface in a browser.
- Toggle the system state (ON/OFF) using the checkbox.
- Control the alarm and light by clicking the respective buttons.
- View motion detection notifications when the alarm and light are activated.
## Troubleshooting
- Ensure the microcontroller is connected to the correct Wi-Fi network and Firebase is set up correctly.
- Check the Firebase Realtime Database to ensure that the values for alarm, light, and sensor are being updated correctly.
- If the system is not responding, try restarting both the web interface and the microcontroller.
## License
This project is licensed under the MIT License.

Feel free to contribute and improve this project!
