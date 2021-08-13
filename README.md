- [Introduction](#introduction)
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

## Project Details:

The modules below are used for this project.

### Components

#### MCU
I used **STM32F103VET6** as the MCU of this project. I used Keil as my developing tool, all codes are coded in **C**, using the **StdPeriph** library of ST.

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/ee96a469af56a7de4c5dc01b580b70b.jpg)

#### Fingerprint Module
I used AS608 for finger print module. AS608 communicxate with MCU using **USART**, it can read fingerprint, generate characteristic, store it in buffer, and compare two characteristic file.

![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/AS608.jpg)

#### Oled Module
0.96 inch OLED screen that uses **I2C** to communicate with MCU. Will be used for UI.
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/oled.jpg)

#### Keyboard Module
A 4x4 button module will be used as the keyboard of the system.
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/keyboard.jpg)

#### Step Motor Module
A step motor module will be used to simulate the mechanical part of a actual lock. A ULN2003 Board is used to control the motor.
![](https://github.com/jiwu66/Smart_Lock_Project/blob/main/image/motor.jpg)
### Design Schematic

## Code

This is an h1 heading

### Driver files

#### AS608

#### OLED

#### 4x4 Keyboard

#### Step Motor

