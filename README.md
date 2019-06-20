# Magnitie - The Metal Finder Robot

## Introduction

This repository contains information about my custom final project for the class CS/EE 120B - Introduction to Embedded Systems at University of California, Riverside. I took this course on spring of 2019. From this course, I gained critical knowledge about hardware and software design of digital computing systems embedded in electronic devices, embedded processor programming, microcontrollers, standard peripherals, and hardware/software tradeoffs. My goal was to apply as many of these topics as possible in the development of this project.

## High Level Description

Magnitie is a four-wheel robot capable of finding small size metal objects such as coins, clips, nails, and keys as he travels. The user controls Magnitie with a button that makes it start moving only in a straight direction. When traveling, if the inductive sensor of Magnitie detects a metal object, Magnitie stops so that the user can pick up the metal object found. After stopping, Magnitie waits for the user to make it start moving again. If Magnitie is ever close to collide with an obstacle, it will stop so that the user can either remove the obstacle or change its direction. Furthermore, Magnitie displays a smiling face on its LCD after finding a metal object and a sad face if it gets stopped due to an obstacle.

![Magnitie](https://github.com/johan1505/Magnitie---The-Metal-Detector-Finder/blob/master/Images/20190614_150317.jpg?raw=true)

## User Guide

- Magnitie is turned on by plugging in its battery wires to its breadboard. When turned on, it waits for user to press the start button
- Magnitie starts to move by pressing the start button that is located next to its LCD
- Magnitie comes with the HC-S04 ultrasonic sensor which enables it to detect potential obstacles on its way
- If before start moving the ultrasonic sensor detects an obstacle 19 (or less) inches away  from its front side, it will not start its trajectory even if the start button is being pressed, and it will display a warning about the obstacle on its LCD.

![OBSTACLE DETECTED](https://github.com/johan1505/Magnitie---The-Metal-Detector-Finder/blob/master/Images/Obstacle1.jpg?raw=true)

- Once Magnitie starts moving, it travels in a straight line and displays a its face on the LCD
- Once in movement, the robot can stop for two reasons:
	- Detecting a metal object
		- If Magnitie finds a metal object, it stops, increases the number of metals found that is displayed on the LCD, and displays a happy face for 3 seconds
		- During these three seconds, the robot will not start to move even the user presses the start button.
		- After the three seconds have passed, the robot changes its face back to normal and waits for its button to be pressed in order to start to move again. 
		- If the robot detects a metal and stops right on top of it, the counter will only increase once, and the robot will not start to move until the found metal is removed from the range of the inductive sensor 

		![METAL DETECTED AND HAPPY FACE](https://github.com/johan1505/Magnitie---The-Metal-Detector-Finder/blob/master/Images/Clear.jpg?raw=true)

	- Detecting an obstacle
		- If the ultrasonic sensor of Magnitie detects an obstacle 19 (or less) inches away from its front side, the robot stops, and it displays a sad face and an “Obstacle detected” warning on its LCD (under the counter of metals found). 
		- Magnitie will not move as long as the sensor keeps detecting an obstacle in front of it, even if the user presses the start button. 
		- If a metal object happens to pass under the inductive sensor, the counter of found metals will increase but the LCD will still display a sad face and the warning.   

		![OBSTACLE DETECTED AND SAD FACE](https://github.com/johan1505/Magnitie---The-Metal-Detector-Finder/blob/master/Images/Obstacle2.jpg?raw=true)

## Components Used

### Hardware & Technologies Used
Note: The equipment that was not taught in this course has been bolded.

- Atmega 1284 microcontroller
- Atmel Studio 7.0 software
- **URBEST Inductive proximity sensor**
- **HC-SR04 Ultrasonic sensor**
- **2 DC motors**
- 4 plastic wheels
- **Onyehn DC motor speed controller**
- **9v battery**
- **1 Voltage regulator**
- **2 MOSFET transistors**
- 16x2 LCD
- Jumper wires
- Button
- Breadboards

### (Pin-out)
- Inputs:
	- Inductive proximitty sensor	
	- Proximity sensor
	- Start button
	- Ultrasonic sensor echo

- Outputs
	- LCD screen
	- Motors
	- Ultrasonic sensor trigger

	![PINOUT](https://github.com/johan1505/Magnitie---The-Metal-Detector-Finder/blob/master/Images/PINOUT.png?raw=true)