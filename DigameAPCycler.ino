//****************************************************************************************
// Digame Includes. (Usually found in ...Arduino\libraries\DigameUtils folder)
//****************************************************************************************
#include <digameDebug.h>        // Serial debugging defines. 
//#include <digameFile.h>       // Read/Write Text files.
#include <digameNetwork_v2.h>   // For MAC address functions
#include "ESPAsyncWebServer.h"

// Servo stuff to trigger the proximity detector
#include <Servo.h>
static const int servoPin = 4;
Servo servo1; 

// Set web server port number to 80
AsyncWebServer server(80);


// Declares
void  showSplashScreen();
void  configureAccessPoint(String netName);
void sweepServo();

String reportingLocation = "Reporting Loc";



//****************************************************************************************
void setup() {
//****************************************************************************************
  Serial.begin(115200);   // Intialize terminal serial port
  delay(1000);            // Give port time to initalize
  showSplashScreen();

  configureAccessPoint(reportingLocation);

  servo1.attach(servoPin);
  
  server.on(
    "/post",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
      }
 
      Serial.println();
 
      request->send(200);
  });
 
  server.begin();
 
}


int    i = 1;
int    numStops = 3;
long   stopDelay = 20; // Time between switching locations
int    secondsPassed = 0;
String header;
bool   servoActive = false;

//****************************************************************************************
void loop() {
//****************************************************************************************

  if (i>numStops) {
    servoActive=false;
    configureAccessPoint(reportingLocation);
    i = 0; 
  } else {
    servoActive=true;
    configureAccessPoint("Stop " + String(i)); // Simulate a WiFi AP for a shuttle stop.
  }
  
  secondsPassed=0;
  DEBUG_PRINT("    ");
  i++;

  do {
    DEBUG_PRINT(".");
    if (servoActive){
      sweepServo();
    } else {
      delay(4000);
    }
    secondsPassed++;
  } while(secondsPassed<stopDelay);
  
  DEBUG_PRINTLN();
 
}



//****************************************************************************************
void configureAccessPoint(String netName){
//****************************************************************************************
  DEBUG_PRINTLN("  Setting AP SSID: " + netName);  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(netName.c_str(),"ReportingPW");
  IPAddress IP = WiFi.softAPIP();
  server.begin();  
}



//****************************************************************************************
void showSplashScreen(){
//****************************************************************************************
  String compileDate = F(__DATE__);
  String compileTime = F(__TIME__);

  DEBUG_PRINTLN();
  DEBUG_PRINTLN("*******************************************");
  DEBUG_PRINTLN("Access Point SSID Cycler");
  DEBUG_PRINTLN("Version 1.0");
  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("Compiled: " + compileDate + " at " + compileTime); 
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("A program to simulate moving through an area");
  DEBUG_PRINTLN("with multiple WiFi Access points.");
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Copyright 2022, Digame Systems.");
  DEBUG_PRINTLN("All rights reserved.");
  DEBUG_PRINTLN("*******************************************");
  DEBUG_PRINTLN();   
}

//****************************************************************************************
void sweepServo(){
//**************************************************************************************** 
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
      servo1.write(posDegrees);
      //Serial.println(posDegrees);
      delay(5);
  }
  
  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
      servo1.write(posDegrees);
      //Serial.println(posDegrees);
      delay(5);
  }
}
