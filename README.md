- [Introduction](#introduction)
- [Function Exhibion](#function-exhibition)
    * [System Initialization](#system-initialization)
    * [Admin Password Setting](#admin-password-setting)
    * [Admin Mode Entering](#admin-mode-entering)
    * [Fingerprint Recording](#fingerprint-recording)
    * [Delete Fingerprints](#delete-fingerprints)
    * [Changing Admin Password](#changing-admin-password)
    * [Unlock by Scanning Fingerprint](#unlock-by-scanning-fingerprint)
- [Project Details](#project-details)
  * [Components](#components)
    + [MCU](#mcu)
    + [Fingerprint Module](#fingerprint-module)
    + [OLED Module](#oled-module)
    + [Keyboard Module](#keyboard-module)
    + [Step Motor Module](#step-motor-module)
  * [Design Schematic](#design-schematic)
- [Code](#code)
  * [Driver files](#driver-files)
    + [AS608](#as608)
    + [OLED](#oled)
    + [4x4 Keyboard](#4x4-keyboard)
    + [Step Motor](#step-motor)
- [Conclusion and Possible Improvement](conclusion-and-possible-improvemeent)

# Heading levels

> This is a fixture to test heading levels

<!-- toc -->

## Introduction
As technology grows, people now don't need to remember to bring their keys with them every time.
Numerous new kinds of lock are being invented and produced to help people create a safer and more convinient life for their family and themselves.\
In this project, I am going to create one of those locks myself. The lock uses fingerprints to identify the users and unlock. The project contains a MCU, a finger print module,
a 4x4 keyboard, a OLED screen, and a step motor. Details of each motor can be found in the sections below.
![](image/Project.jpg)

**(All project files and codes will be included in the prository)**

## Function Exhibition
This section demonstrate each function of the system

### System Initialization
Everytime the system is powered up, the system will check if an **admin password** has been set. If not, then it is the first-time using the system and it will ask user to set an admin password. After system initialization, it will enter **Main Menu**\
The admin password is stored in **SPI Flash**, so that the **data will not be lost if power is shut down**. 
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/init.gif)

### Admin Password Setting
The system will ask user to set an admin password if it is the first-time using the system. The password should be 8-digit, a key board supporting number 0 - 9 is provided.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/password_setting.gif)

### Admin Mode Entering
Admin Mode can be entered by entering the correct Admin password.\
Admin mode contains three functions:\
**1.Add Fingerprint**
**2.Delete Fingerprints**
**3.Change Password**
**4.Back to Menu**\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/admin_mode1.gif)

### Fingerprint Recording
If **Add Fingerprint** is selected in Admin Mode, the system will ask the user to put the finger desired onto the fingerprint module to scan twice. The characteristic of the finger print will **only be generated and stored if the two scannig match each other**.\
After characteristic has been generated, the user will be asked to enter an ID number to assign to the model.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/add_fp_p1.gif)
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/add_fp_p2.gif)

### Delete Fingerprints
If **Delete Fingerprints** has been selected in Admin Mode, the system will **delete all fingerprints stored**.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/delete_fp.gif)

### Changing Admin Password
If **Change Password** is selected in Admin Mode, the system will ask the user to set another password(8-digit).\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/change_password.gif)

### Unlock by Scanning Fingerprint
If **Unlock** has been selected in Menu, the system will ask the user to put finger onto fingerprint module to scan. If the fingerprint match with any characteristic stored, it will unlock by controling the step motor.\
The user **can press any key to exit to menu**.\
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/unlcok.gif)

## Project Details:

The modules below are used for this project.

### Components

#### MCU
The board I have uses **STM32F103VET6** as the MCU. I used Keil as my developing tool, all codes are coded in **C**, using the **StdPeriph** library of ST.\
The board contains one **SPI FLASH** the flash will be used in this project as well.

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/ee96a469af56a7de4c5dc01b580b70b.jpg)

#### Fingerprint Module
I used AS608 for finger print module. AS608 communicxate with MCU using **USART**, it can read fingerprint, generate characteristic, store it in buffer, and compare two characteristic file.

**Pins Connection:**\
RX:->PA2\
TX:->PA3

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/AS608.jpg)

#### Oled Module
0.96 inch OLED screen that uses **I2C** to communicate with MCU. Will be used for UI.

**Pins Connection:**\
SCL:->PB6\
SDA:->PB7

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/oled.jpg)

#### Keyboard Module
A 4x4 button module will be used as the keyboard of the system.

**Pins Connection:**\
C1:->PB8\
C2:->PC6\
C3:->PB14\
C4:->PB12\
R1:->PB13\
R2:->PB15\
R3:->PC7\
R4:->PB9

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/keyboard.jpg)

#### Step Motor Module
A step motor module will be used to simulate the mechanical part of a actual lock. A ULN2003 Board is used to control the motor.

**Pins Connection:**\
IN1:->PC8\
IN2:->PC9\
IN3:->PC10\
IN4:->PC11

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/motor.jpg)
### Design Schematic

## Code

This is an h1 heading

### Driver files

#### AS608

#### OLED

#### 4x4 Keyboard

#### Step Motor

## Conclusion and Possible Improvement
As a result, the project contains all functions required for a smart fingerprint lock.\
I gained hands on project experience of using STM32 peripherals including GPIO, TIM, Systick. I also learned to use communication protocols like I2C, SPI, and USART to communicate with between MCU and other devices including AS608, OLED, and FLASH.\
I used a developer board with STM32 MCU for this project, it is unecessary if I want to make it an actual product. The board takes up too much space and the system does not require so many pins. It can be improved by selecting a more suitable MCU(can still be STM32f103 series but with less pins). A PCB with much a smaller size can be designed, all modules can be attached on the PCB insted of connecting with wires. I could design a better power supply, the existing system uses USB for power supply, I should peobably use battery if I want to make it a product.
