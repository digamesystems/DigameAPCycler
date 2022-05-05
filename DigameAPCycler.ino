//****************************************************************************************
// Digame Includes. (Usually found in ...Arduino\libraries\DigameUtils folder)
//****************************************************************************************
#include <digameDebug.h>     // Serial debugging defines. 
//#include <digameFile.h>      // Read/Write Text files.
#include <digameNetwork.h>   // For MAC address functions

void  showSplashScreen();
void  configureAccessPoint(String netName);

//****************************************************************************************
void setup() {
//****************************************************************************************
  Serial.begin(115200);   // Intialize terminal serial port
  delay(1000);            // Give port time to initalize
  showSplashScreen();
}



int    i = 1;
int    numStops = 3;
long   stopDelay = 30; // Time between switching locations
int    secondsPassed = 0;
String reportingLocation = "Rept Loc";  
//****************************************************************************************
void loop() {
//****************************************************************************************
  
  if (i>numStops) {
    configureAccessPoint(reportingLocation);
    i = 0; 
  } else {
    configureAccessPoint("Stop " + String(i)); // Simulate a WiFi AP for a shuttle stop.
  }
  
  secondsPassed=0;
  DEBUG_PRINT("    ");
  i++;

  do {
    DEBUG_PRINT(".");
    delay(1000);//Delay a second
    secondsPassed++;
  } while(secondsPassed<stopDelay);
  
  DEBUG_PRINTLN();
  
}



//****************************************************************************************
void configureAccessPoint(String netName){
//****************************************************************************************
  DEBUG_PRINTLN("  Setting AP SSID: " + netName);  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(netName.c_str());
  IPAddress IP = WiFi.softAPIP();
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
