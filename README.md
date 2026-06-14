# STM32-Based Smart Industrial Safety Monitoring System

An advanced STM32 embedded systems project focused on industrial safety monitoring and multi-sensor data processing using STM32 Blue Pill, Embedded C, UART communication, and ADC peripherals.

## Project Overview

This project simulates a smart industrial safety monitoring system capable of:

* Temperature Monitoring
* Gas Leakage Detection
* Real-Time UART Telemetry
* Industrial Warning System
* Multi-Channel ADC Processing
* Embedded Sensor Monitoring

The project is developed using STM32CubeIDE and simulated using PICSimLab.

---

# Features

## Day 1 – Temperature Monitoring System

* ADC-based temperature sensor reading
* UART serial monitoring
* LED warning system
* Real-time sensor processing

## Day 2 – MQ2 Gas Leakage Detection

* Multi-channel ADC implementation
* Gas sensor monitoring
* Gas leakage warning alert
* Industrial safety logic
* Combined temperature + gas telemetry

---

# Hardware / Simulation

## Microcontroller

* STM32F103C8T6 (Blue Pill)

## Simulation Platform

* PICSimLab

## Communication

* UART Serial Communication

## Sensors

* LM35 Temperature Sensor (Simulated)
* MQ2 Gas Sensor (Simulated)

---

# Technologies Used

* Embedded C
* STM32CubeIDE
* STM32 HAL Drivers
* UART Communication
* ADC Peripheral
* GPIO Control
* PICSimLab Simulation

---

# UART Output Example

```text
Temp: 34.22 C | Gas: 52.11 %

WARNING: GAS LEAK DETECTED
```

---

# Project Structure

```text
Core/
Drivers/
Debug/
README.md
.ioc File
```

---

# Future Enhancements

* OLED Display Dashboard
* FreeRTOS Integration
* IoT Cloud Monitoring
* Data Logging System
* Buzzer Alarm System
* Wireless Monitoring
* Mobile App Dashboard

---

# Learning Outcomes

This project helped in understanding:

* STM32 Peripheral Configuration
* Multi-Channel ADC
* UART Communication
* Embedded Sensor Processing
* Industrial Monitoring Systems
* Embedded Firmware Development
* Real-Time Monitoring Logic

---

# Author

Abhiney Kumar

Embedded Systems | IoT | STM32 | Embedded C | Industrial Automation
