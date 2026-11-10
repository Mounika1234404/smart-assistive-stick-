# smart-assistive-stick-
A Smart Assistive Stick for visually impaired individuals featuring moisture detection, obstacle detection, Bluetooth-based companion alerts, and OLED display integration, developed using Embedded C in Keil µVision.


# Smart Assistive Stick for Visually Impaired People

## 📌 Project Overview

The **Smart Assistive Stick** is an embedded systems-based assistive device designed to improve the safety and mobility of visually impaired individuals. It uses sensors, visual indicators, audio alerts, vibration feedback, and Bluetooth communication to help users identify nearby obstacles and wet surfaces and send emergency alerts to companions.

The project is developed using **Embedded C programming in Keil µVision IDE**.

## ✨ Features

### 1. Obstacle Detection
- Uses an ultrasonic sensor to detect nearby obstacles.
- When an obstacle is detected within **70 cm**, the buzzer turns ON and the yellow LED glows.
- When no nearby obstacle is detected, the buzzer turns OFF and the green LED glows.

### 2. Moisture Detection
- Detects wet soil using a moisture sensor.
- When wet soil is detected, the vibration motor turns ON to alert the user through tactile feedback.

### 3. OLED Display
- Displays real-time system status.
- Shows:
  - Obstacle detection status
  - Soil moisture status
  - Vibration motor status

### 4. Emergency Alert via Bluetooth
- Uses the HC-05 Bluetooth module for wireless communication.
- When the emergency button is pressed, an alert is sent through Bluetooth to notify a connected companion device.

## ⚙️ Working Principle

The microcontroller continuously monitors the ultrasonic sensor, moisture sensor, and emergency button.

Based on the sensor readings:

| Condition | System Response |
|---|---|
| Obstacle within 70 cm | Buzzer ON + Yellow LED ON |
| No nearby obstacle | Buzzer OFF + Green LED ON |
| Wet soil detected | Vibration Motor ON |
| Emergency button pressed | Alert sent via HC-05 Bluetooth |
| System operating | OLED displays current statuses |

## 🛠️ Hardware Components

- Microcontroller
- Ultrasonic Sensor
- Soil Moisture Sensor
- HC-05 Bluetooth Module
- OLED Display
- Buzzer
- Yellow LED
- Green LED
- Vibration Motor
- Emergency Push Button
- Connecting Wires
- Power Supply
- stm32 black pill 

## 💻 Software & Technologies

- **Programming Language:** Embedded C
- **IDE:** Keil µVision
- **Communication:** UART / Bluetooth (HC-05) , vibration motor
- **Display Interface:** OLED
- **Sensor Integration:** Ultrasonic and Soil Moisture Sensors

## 🎯 Project Objectives

- To improve independent mobility for visually impaired individuals.
- To detect obstacles and provide immediate alerts.
- To identify wet surfaces using moisture sensing.
- To provide vibration-based alerts for better accessibility.
- To enable emergency communication through Bluetooth.

## 🌍 Applications

- Assistive technology for visually impaired people.
- Smart mobility assistance systems.
- Embedded systems-based safety devices.
- Sensor-based navigation assistance.

## 🚀 Future Enhancements

- GPS-based real-time location tracking.
- Voice guidance and audio navigation.
- Mobile application for emergency notifications.
- GSM-based alerts for longer-range communication.
- Rechargeable battery and power optimization.


## 👩‍💻 Development

This project was developed as an Embedded Systems project using Keil µVision and Embedded C.

**Smart Assistive Stick – Making Navigation Safer Through Technology.**
