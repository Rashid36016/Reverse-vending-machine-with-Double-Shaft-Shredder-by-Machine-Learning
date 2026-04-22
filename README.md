# Reverse Vending Machine (RVM) with Double Shaft Auto/Timed Shredder

## Project Overview

This project presents a Reverse Vending Machine (RVM) designed to collect and process plastic bottles. The system integrates artificial intelligence, embedded systems, and mechanical design to automate validation and shredding.

The machine detects bottles using a YOLO-based model, verifies them using sensors, and processes them through a double shaft shredding mechanism.

---

## Objectives

* Automate plastic bottle collection
* Validate objects using AI and sensors
* Reduce waste volume through shredding
* Develop an integrated mechatronic system

---

## System Architecture

### Detection Layer

The ESP32-S3 CAM captures images and the YOLO model detects whether the object is a valid bottle.

### Validation Layer

Sensors such as IR or proximity sensors confirm the presence and correctness of the object.

### Processing Layer

A double shaft shredder crushes the bottle to reduce volume and prepare it for recycling.

---

## Working Principle

1. User inserts a bottle
2. Image is captured
3. YOLO model detects the object
4. Sensors validate the input
5. If valid, the shredder activates
6. Bottle is crushed
7. System provides output feedback

---

## Shredder System

The machine uses a double shaft shredding mechanism where two shafts rotate in opposite directions. Blades attached to the shafts grip and cut the bottle, improving efficiency and reducing jamming.

---

## Control System

* Automatic mode: activates when a valid object is detected
* Timed mode: runs for a fixed duration to prevent overload

---

## Hardware Components

* ESP32
* ESP32-S3 CAM
* Motor driver
* DC motors
* Sensors (IR / proximity)
* Thermal printer
* Power supply

---

## Software

* Arduino IDE
* Python (YOLO)
* Embedded C/C++

---

## Applications

* Smart recycling systems
* Educational institutions
* Public places
* Smart city projects
