# DigameAPCycler
A test program to cycle an ESP32 through a number of Access Point SSIDs. -- Emulates moving through an area with known WiFi hot spots.

## Overview
Several of our mobile applications use a list of known WiFi SSIDs as crude location beacons to give us an idea of where we are. This app helps us test the logic by broadcasting on a number of SSIDs in sequence, pausing at each one for an adjustable amount of time. 

Our application also uses a known "Reporting SSID" to which we connect and send data to our server. While this app wont actually let the device send data to our server, it will provide a "Reporting SSID" we can use to test the failure recovery logic when reporting goes awry. 

Support for a servo motor is included to provide signals to the proximity sensor under test. Servo motor control is provided via Jaroslav Paral's [ServoESP32](https://github.com/RoboticsBrno/ServoESP32) library.
