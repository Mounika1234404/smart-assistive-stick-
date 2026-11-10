# Smart Assistive Stick for Visually Impaired People

## 1. Project Overview

The **Smart Assistive Stick** is an embedded systems-based assistive device designed to improve the safety, mobility, and independence of visually impaired individuals.

The system integrates sensors, audio alerts, visual indicators, vibration feedback, an OLED display, and Bluetooth communication to detect obstacles, identify wet surfaces, and send emergency alerts to a companion.

The project is implemented using **Embedded C programming in Keil µVision IDE** with the **STM32F401CCU6 microcontroller**.

## 2. Project Objectives

- Improve independent mobility for visually impaired individuals.
- Detect nearby obstacles and provide immediate alerts.
- Identify wet soil or surfaces using a moisture sensor.
- Provide vibration-based feedback to the user.
- Enable emergency communication through Bluetooth.
- Display real-time system information using an OLED display.

## 3. Key Features

### 3.1 Obstacle Detection
- Uses an HC-SR04 ultrasonic sensor to detect nearby obstacles.
- If an obstacle is detected within **70 cm**, the buzzer turns ON and the yellow LED glows.
- If no nearby obstacle is detected, the buzzer turns OFF and the green LED glows.

### 3.2 Moisture Detection
- Uses a soil moisture sensor to detect wet soil.
- When wet soil is detected, the vibration motor turns ON to alert the user through vibration feedback.

### 3.3 OLED Display
The OLED display provides real-time information about:
- Obstacle detection status.
- Soil moisture status.
- Vibration motor status.

### 3.4 Emergency Alert Using Bluetooth
- Uses the HC-05 Bluetooth module for wireless communication.
- When the emergency button is pressed, an alert is transmitted through Bluetooth to a connected companion device.

## 4. Hardware Components

| S.No. | Component |
|:---:|---|
| 1 | STM32F401CCU6 Microcontroller |
| 2 | HC-SR04 Ultrasonic Sensor |
| 3 | Soil Moisture Sensor |
| 4 | HC-05 Bluetooth Module |
| 5 | OLED Display |
| 6 | Buzzer |
| 7 | Yellow LED |
| 8 | Green LED |
| 9 | Vibration Motor |
| 10 | Emergency Push Button |
| 11 | Resistors |
| 12 | Transistor/MOSFET Driver Circuits |
| 13 | Connecting Wires |
| 14 | Power Supply |

## 5. Hardware Connections

The following table represents the hardware connections with the STM32F401CCU6 microcontroller.

| S.No. | Hardware Component | Module Pin | STM32F401CCU6 Pin |
|:---:|---|---|:---:|
| 1 | HC-SR04 Ultrasonic Sensor | TRIG | PA1 |
| 2 | HC-SR04 Ultrasonic Sensor | ECHO | PA0 |
| 3 | HC-SR04 Ultrasonic Sensor | VCC | 5V |
| 4 | HC-SR04 Ultrasonic Sensor | GND | GND |
| 5 | Soil Moisture Sensor | AO | PA4 |
| 6 | Soil Moisture Sensor | VCC | 3.3V or module-rated supply |
| 7 | Soil Moisture Sensor | GND | GND |
| 8 | Vibration Motor Driver | IN | PB4 |
| 9 | Green LED | Anode (+) through resistor | PB5 |
| 10 | Buzzer Driver | IN | PB6 |
| 11 | Yellow LED | Anode (+) through resistor | PB7 |
| 12 | Emergency Push Button | One terminal | PC13 |
| 13 | Emergency Push Button | Other terminal | GND |

**Important Connection Notes:**

1. **HC-SR04 ECHO:** The ECHO output can be 5V. Use a voltage divider or level shifter before connecting it to PA0 because STM32 GPIO pins are not 5V tolerant in every operating condition.
2. **Vibration Motor and Buzzer:** Use suitable transistor/MOSFET driver circuits. Do not connect the motor directly to the GPIO pin.
3. **LED Connections:** Use current-limiting resistors, typically 220–330 Ω.
4. **Common Ground:** Connect the STM32, sensor modules, and driver circuits to a common ground.
5. **Emergency Button:** PC13 uses the internal pull-up configuration in the code. Pressing the button connects PC13 to GND.

*Note: HC-05 and OLED pin connections are not included here because their exact pin assignments have not been specified in the hardware connection document.*

## 6. Working Principle

The STM32F401CCU6 microcontroller continuously monitors the ultrasonic sensor, soil moisture sensor, and emergency push button.

Based on the input conditions, the microcontroller activates the corresponding output devices.

| Input Condition | Output Response |
|---|---|
| Obstacle within 70 cm | Buzzer ON + Yellow LED ON |
| No nearby obstacle | Buzzer OFF + Green LED ON |
| Wet soil detected | Vibration Motor ON |
| Emergency button pressed | Alert sent through HC-05 Bluetooth |

The OLED display continuously provides information about obstacle detection, soil moisture, and vibration status.

## 7. Software and Technologies

| Technology | Purpose |
|---|---|
| Embedded C | Programming language |
| Keil µVision IDE | Code development and compilation |
| STM32F401CCU6 | Main microcontroller |
| HC-SR04 | Obstacle detection |
| Soil Moisture Sensor | Moisture detection |
| HC-05 | Bluetooth communication |
| OLED Display | System status display |

## 8. Project Structure

```text
Smart-Assistive-Stick/
│
├── main.c
├── README.md
└── Images/
    └── project_setup.jpg
```

## 9. Applications

- Assistive technology for visually impaired individuals.
- Smart mobility assistance systems.
- Obstacle detection and safety systems.
- Embedded systems-based accessibility solutions.

## 10. Future Enhancements

- GPS-based real-time location tracking.
- Voice-based navigation assistance.
- Mobile application integration.
- GSM-based emergency notifications.
- Rechargeable battery and power optimization.

## 11. Team Details

| S.No. | Student Name | Registration Number |
|:---:|---|---|
| 1 | [Student Name 1] | [Registration Number 1] |
| 2 | [Student Name 2] | [Registration Number 2] |
| 3 | [Student Name 3] | [Registration Number 3] |
| 4 | [Student Name 4] | [Registration Number 4] |

## 12. Academic Details

**Mentor:** Dr. A. Baskar

**Course:** 23CSE304 – Embedded Systems

**Department:** Department of Computer Science and Engineering

**Institution:** School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore, India.

## 13. Acknowledgement

We sincerely express our gratitude to our mentor, **Dr. A. Baskar**, for the valuable guidance, encouragement, and support provided throughout the development of this project.

We also acknowledge the **Department of Computer Science and Engineering, School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore**, for providing the academic environment and opportunity to carry out this project as part of the course **23CSE304 – Embedded Systems**.

This project has provided us with practical experience in Embedded C programming, sensor interfacing, microcontroller-based system design, and wireless communication.

---

**Smart Assistive Stick – Making Navigation Safer Through Technology.**
