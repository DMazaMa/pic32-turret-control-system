# PIC32 Turret Control System
**Manual and Automatic Turret Control using PIC32MX230F064D**

## Overview
This repository contains the implementation of a **manual and automatic turret control system** developed using the **PIC32MX230F064D microcontroller**.  
The project was created as a **final academic project for the Microprocessors course**, with a strong focus on **low-level embedded programming**, **modular driver design**, and **real-time hardware control**.

The system integrates multiple peripherals such as **ultrasonic sensing, PWM-based motor control, servo positioning, ADC joystick input, and UART communication**, all coordinated through a **finite state machine (FSM)**.

---

## Features
- Dual operation modes: **Automatic** and **Manual**
- Object detection using **HC-SR04 ultrasonic sensor**
- Turret positioning via **servo motors**
- Shooting mechanism controlled by **PWM-driven DC motor**
- **Joystick-based manual control** using ADC
- **UART communication** for monitoring and mode switching
- Modular and scalable driver-based architecture
- Finite State Machine for deterministic control logic

---

## Hardware Components
- **Microcontroller:** PIC32MX230F064D
- **Ultrasonic Sensor:** HC-SR04
- **DC Motor:** Shooting mechanism
- **Servo Motors (2x):** Turret direction and firing barrier
- **Joystick (Potentiometer):** Manual control
- **UART Interface:** Debugging and mode selection
- **Push Button:** Manual firing trigger
- **Laser Module:** Visual firing indicator

---

## Project Structure
.
├── drivers
│ ├── hcsr04.c / hcsr04.h # Ultrasonic sensor driver
│ ├── motor.c / motor.h # PWM motor control
│ ├── ModuloServo.c / ModuloServo.h # Servo control module
│ ├── potenciometro.c / .h # ADC joystick driver
│ ├── uart.c / uart.h # UART communication
│ ├── Retardos.c / Retardos.h # Timing and delays
│ ├── Pic32Ini.c / Pic32Ini.h # MCU initialization
│ ├── mainfinal.c # Main application logic
│ └── Makefile
│
├── test
│ ├── main_joystick.c # Joystick test
│ ├── main_prueba_motor.c # Motor test
│ ├── mainprueba.c # General tests
│ ├── mainservos.c # Servo tests
│ └── mainultrasonidos.c # Ultrasonic sensor tests
│
└── README.md


---

## Operating Modes

### Automatic Mode
- The turret performs a continuous horizontal scan
- Distance is measured using the ultrasonic sensor
- When an object is detected within a predefined range:
  - The system waits for user confirmation
  - The shooting mechanism is activated
- After firing, the system resets and resumes scanning

### Manual Mode
- Turret direction is controlled using the joystick
- Distance measurement is used to adjust motor behavior
- Firing is triggered using a push button

---

## Control Logic
The system logic is implemented in `mainfinal.c` and follows this structure:
1. Peripheral initialization
2. Continuous main loop execution
3. Finite State Machine handling system behavior
4. Real-time interaction with hardware peripherals

This approach ensures **deterministic behavior**, **code readability**, and **easy scalability**.

---

## Technologies & Skills Demonstrated
- Embedded C programming
- PIC32 microcontroller architecture
- PWM and Output Compare modules
- ADC configuration and signal processing
- UART serial communication
- Modular driver development
- Finite State Machine design
- Bare-metal programming
- Real-time embedded systems

---

## Academic Context
Developed as a **Final Project for the Microprocessors course**  
**Universidad Pontificia Comillas (ICAI)**

**Authors:**

- Diego Luis de Mazarredo Martín
- Jaime Martínez Martínez

---

## License
This project is intended for **educational and portfolio purposes**.
