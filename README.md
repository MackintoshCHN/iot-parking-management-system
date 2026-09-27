# IoT-Based Parking Management System

An STM32-based IoT parking management system integrating parking space monitoring, parking guidance, fee calculation, TFT display, servo control, Bluetooth communication, and mobile app interaction.

This project was developed as my **Bachelor of Engineering graduation project in Internet of Things Engineering in 2024**. The system integrates embedded hardware, sensors, local display, actuator control, and wireless communication into a working parking management prototype.

## Prototype Overview

The prototype monitors three parking spaces (A, B, and C), detects vehicle occupancy, provides parking guidance, calculates parking fees, displays parking information on a TFT screen, and synchronises status information with a mobile application.

![Prototype Overview](images/prototype_overview.jpg)

## Key Features

- Vehicle entry and parking-space occupancy detection
- Monitoring of three parking spaces: A, B, and C
- Automatic parking-space guidance based on current availability
- Parking fee calculation based on parking duration
- TFT display for parking status, fees, and guidance information
- Servo-based gate simulation
- Bluetooth communication with a mobile application
- Mobile monitoring of parking-space status and parking fees
- Remote fee-clearing and gate-control operations

## System Requirements

The system was designed around three groups of requirements:

- **Functional requirements:** entry/exit management, parking-space monitoring, fee management, parking lock control, and parking guidance
- **Performance requirements:** real-time response, accuracy, stability, and reliability
- **Feasibility requirements:** technical, economic, and operational feasibility

![System Requirements](images/system_requirements.png)

## System Architecture

The system uses a main controller to coordinate the parking entry/exit module, parking-space monitoring, fee management, parking lock control, and parking guidance functions.

A TFT display provides local information, while Bluetooth communication connects the embedded system with the mobile application.

![System Architecture](images/system_architecture.png)

## System Functions

The overall functionality is divided into three main areas:

### Data Acquisition

- Parking entry/exit detection
- Parking-space occupancy monitoring

### System Control

- Parking guidance
- Parking lock control
- Servo motor control
- Bluetooth communication

### Data Display

- TFT display of parking status and system information

![System Functions](images/system_functions.png)

## System Workflow

The embedded application continuously processes parking status and user input.

The main workflow includes:

1. GPIO and display initialisation
2. Parking-space status monitoring
3. Vehicle-entry detection
4. Servo and parking-guidance control
5. Parking-status confirmation
6. Parking fee calculation
7. Data updates and communication
8. Vehicle-exit and billing completion handling

![System Workflow](images/system_workflow.png)

## Hardware Prototype

The prototype integrates an STM32-based controller with infrared detection modules, a TFT display, Bluetooth communication hardware, a servo motor, control buttons, and an audio output module.

![Hardware Overview](images/hardware_overview.jpg)

## Mobile Application

The mobile interface displays the occupancy state of parking spaces A, B, and C together with their corresponding parking fees.

It also provides control functions for fee clearing and gate operation through the wireless connection.

![Mobile Application](images/mobile_app.png)

## Parking Status Display

The TFT screen provides local parking information, including:

- Parking-space identifiers
- Occupancy status
- Parking fees
- Parking guidance information

![TFT Parking Status](images/tft_parking_status.jpg)

## Vehicle Detection Demo

The following demonstration shows the system detecting a vehicle in parking space A. The mobile application updates the corresponding parking-space state while the embedded system tracks the parking status.

![Vehicle Detection Demo](images/vehicle_detection_demo.jpg)

## Billing Demo

When a vehicle remains in a parking space, the embedded application tracks the parking duration and updates the corresponding parking fee.

The mobile application displays the current fee for each parking space.

![Billing Demo](images/billing_demo.jpg)

## Firmware

The main application logic is located in:

```text
src/main.c
```

The application-level logic covers:

- USART initialisation
- TFT initialisation and display updates
- Parking-space sensor reading
- Vehicle-entry detection
- Parking fee timing and calculation
- Parking guidance selection
- Servo control
- Voice playback triggering
- Key input processing
- Wireless data handling

## Repository Structure

```text
iot-parking-management-system/
│
├── README.md
├── src/
│   └── main.c
│
└── images/
    ├── billing_demo.jpg
    ├── hardware_overview.jpg
    ├── mobile_app.png
    ├── prototype_overview.jpg
    ├── system_architecture.png
    ├── system_functions.png
    ├── system_requirements.png
    ├── system_workflow.png
    ├── tft_parking_status.jpg
    └── vehicle_detection_demo.jpg
```

## Project Information

- **Project:** Bachelor Graduation Project
- **Field:** Internet of Things Engineering
- **Year:** 2024
- **Platform:** STM32
- **Main Components:** STM32 controller, infrared sensors, TFT display, servo motor, Bluetooth communication, mobile application
