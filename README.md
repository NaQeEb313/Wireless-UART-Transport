# Wireless-UART-Transport

A wireless communication project using ESP32 and 433 MHz RF modules to transfer data between two computers.



## Introduction

The aim of this project is to send data from one computer to another without using wires. Data is first sent from a computer to an ESP32, which acts as the transmitter. The ESP32 sends the data through a 433 MHz RF module. On the receiver side, another ESP32 receives the data through an RF receiver module and sends it back to the computer, where it is displayed.

Since RF modules are affected by noise and interference, a simple framing method using start and end symbols is used so that only valid messages are received.



## Objectives

- Design a simple wireless communication system using ESP32 and RF modules.
- Transmit data between two devices without wires.
- Understand UART communication between microcontrollers.
- Study the effect of noise and errors in RF communication.
- Reduce transmission errors using start and end markers.
- Display the received data on the computer.

  

## Software Requirements

- Arduino IDE
- Visual Studio Code
- Windows

  

## Hardware Used

- 2 × ESP32 Development Boards
- 433 MHz RF Transmitter Module
- 433 MHz RF Receiver Module
- USB Cables
- Arduino Uno (5V power supply for RF modules)
- Breadboard
- Jumper Wires
- PC/Laptop

## Prototype

<img width="794" height="1280" alt="image" src="https://github.com/user-attachments/assets/a6323a07-8699-4701-80aa-0307f9470025" />


## How it Works

The system is divided into a transmitter section and a receiver section.

<img width="1080" height="1086" alt="image" src="https://github.com/user-attachments/assets/23aa5d26-b0ef-4e52-8fdb-aefb89450db7" />




### Transmitter

- The user enters data on the computer.
- The data is sent to the ESP32 through USB serial communication.
- The ESP32 sends the data to the RF transmitter module using UART.
- The RF transmitter converts the digital data into radio signals and transmits them wirelessly.

  

### Communication Channel

- Communication takes place over a 433 MHz RF link.
- The signal travels through air without any physical connection.
- Noise and interference may affect the transmitted signal.

  

### Receiver

- The RF receiver module receives the radio signal.
- The received data is sent to the ESP32 through UART.
- The ESP32 processes the data and sends it to the computer.
- The received message is displayed on the serial monitor.

  

## Communication Range

The system uses 433 MHz RF transmitter and receiver modules for wireless communication.

  - **Theoretical Range:** Up to 100 meters in an open area (module specification).
  - **Practical Range:** Not measured as part of this project. The actual communication range depends on antenna quality, power supply, obstacles, and environmental interference.



## Data Flow

```
PC
  │
  ▼
ESP32 (Transmitter)
  │
  ▼
RF Transmitter
  │
  ▼
Wireless (433 MHz)
  │
  ▼
RF Receiver
  │
  ▼
ESP32 (Receiver)
  │
  ▼
PC
```


## Challenges

- Maintaining synchronization between the transmitter and receiver during communication.
- Communication errors were observed at higher UART baud rates.
- Reducing the baud rate to **2400 bps** improved communication reliability and reduced transmission errors.



During testing, a few issues were encountered:

- Synchronization between the transmitter and receiver was difficult to maintain during continuous communication.
- Higher UART baud rates resulted in corrupted data and communication errors.

<img width="1080" height="1083" alt="image" src="https://github.com/user-attachments/assets/06d70a84-ae6b-45fa-b739-03f82bda0229" />



- Reducing the UART baud rate to **2400 bps** improved communication stability and significantly reduced transmission errors.



## Future Improvements

- Replace the ESP32 with an 8051 microcontroller to reduce the overall system cost.
- Use an external UART interface (such as MAX232 or a USB-to-Serial converter) for communication with the computer.
- Connect the 433 MHz RF transmitter and receiver modules to the 8051 UART pins.
- Modify the firmware to support packet creation, checksum verification, and UART communication on the 8051.
- Optimize the packet handling to fit within the limited RAM and Flash memory available on the 8051.

The system uses half-duplex communication, which means data is transmitted in one direction at a time.
