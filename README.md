# Smart Assistive Stick for Visually Impaired People

## 1. Project Overview

The **Smart Assistive Stick** is an embedded systems-based assistive device designed to improve the safety, mobility, and independence of visually impaired individuals.

The system uses sensors, audio alerts, visual indicators, vibration feedback, and Bluetooth communication to detect obstacles, identify wet surfaces, and send emergency alerts to a companion.

The project is implemented using **Embedded C programming in Keil µVision IDE**.

## 2. Project Objectives

- To improve the independent mobility of visually impaired individuals.
- To detect nearby obstacles and provide immediate alerts.
- To identify wet soil or surfaces using a moisture sensor.
- To provide vibration-based feedback to the user.
- To enable emergency communication through Bluetooth.
- To display real-time system information using an OLED display.

## 3. Key Features

### 3.1 Obstacle Detection
- Uses an ultrasonic sensor to detect nearby obstacles.
- If an obstacle is detected within **70 cm**, the buzzer turns ON and the yellow LED glows.
- If no nearby obstacle is detected, the buzzer turns OFF and the green LED glows.

### 3.2 Moisture Detection
- Uses a soil moisture sensor to detect wet soil.
- When wet soil is detected, the vibration motor turns ON to alert the user.

### 3.3 OLED Display
Displays real-time information about:
- Obstacle detection status.
- Soil moisture status.
- Vibration motor status.

### 3.4 Emergency Alert Using Bluetooth
- Uses the HC-05 Bluetooth module for wireless communication.
- When the emergency button is pressed, an alert is transmitted through Bluetooth to a connected companion device.

## 4. Hardware Components

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

## 5. Software and Technologies

| Technology | Purpose |
|---|---|
| Embedded C | Programming |
| Keil µVision IDE | Code development and compilation |
| Ultrasonic Sensor | Obstacle detection |
| Soil Moisture Sensor | Moisture detection |
| HC-05 | Bluetooth communication |
| OLED | Status display |

## 6. Working Principle

The microcontroller continuously monitors the sensor readings and emergency button input.

Based on the detected conditions, the system activates the corresponding output devices.

| Input Condition | Output Response |
|---|---|
| Obstacle within 70 cm | Buzzer ON, Yellow LED ON |
| No nearby obstacle | Buzzer OFF, Green LED ON |
| Wet soil detected | Vibration Motor ON |
| Emergency button pressed | Alert sent through HC-05 Bluetooth |

The OLED display continuously provides information about obstacle, soil moisture, and vibration status.

## 7. Applications

- Assistive technology for visually impaired individuals.
- Smart mobility assistance.
- Obstacle detection and safety systems.
- Embedded systems-based accessibility solutions.

## 8. Future Enhancements

- GPS-based location tracking.
- Voice-based navigation assistance.
- Mobile application integration.
- GSM-based emergency notifications.
- Rechargeable battery and power optimization.

## 9. Project Structure

```text
Smart-Assistive-Stick/
│
├── main.c
├── README.md
└── Images/
    └── project_setup.jpg
```

## 10. Academic Details

**Student Name(s) and Registration Number(s):**

1. [Mounika Dirisala] – [CB.SC.U4CSE24508]
2. [Sudhi Madhuri R] – [CB.SC.U4CSE24540]
3. [G Rajendra] – [CB.SC.U4CSE24511]

**Mentor:** Dr. A. Baskar

**Course:** 23CSE304 – Embedded Systems

**Department:** Department of Computer Science and Engineering

**Institution:** School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore, India.

## 11. Acknowledgement

We sincerely express our gratitude to our mentor, **Dr. A. Baskar**, for the valuable guidance, encouragement, and support provided throughout the development of this case study.

We also acknowledge the **Department of Computer Science and Engineering, School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore**, for providing the academic environment and opportunity to work on this Embedded Systems project as part of the course **23CSE304 – Embedded Systems**.

This project has provided us with practical exposure to embedded C programming, sensor interfacing, microcontroller-based system design, and wireless communication.

---

**Smart Assistive Stick – Making Navigation Safer Through Technology.**
