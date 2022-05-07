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
void showSplashScreen();
void configureAccessPoint(String netName);
void sweepServo();
void configurationMenu();

String reportingLocation = "Reporting Loc";

int      numStops = 3;
long int waitTime = 30;
long int travelTime = 22;


//****************************************************************************************
void setup() {
//****************************************************************************************
  Serial.begin(115200);   // Intialize terminal serial port
  delay(1000);            // Give port time to initalize
  showSplashScreen();
  configurationMenu();
  
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


bool   servoActive = false;

void dotDelay(long int delayTime){
  DEBUG_PRINT("  ");
  for (int i = 0; i<delayTime; i++){
    DEBUG_PRINT(".");
    delay(1000);  
  }  
  DEBUG_PRINTLN();
}

//****************************************************************************************
void loop() {
//****************************************************************************************

  for (int i = 1; i<=numStops; i++){
    configureAccessPoint("Stop " + String(i)); // Simulate a WiFi AP for a shuttle stop.
    dotDelay(waitTime);
    configureAccessPoint(String("Foo")); //Unknown value
    dotDelay(travelTime);
  }

  configureAccessPoint(reportingLocation);
  dotDelay(30);

  /*

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
  */
  DEBUG_PRINTLN();
 
}


void showMenu(){
  Serial.println("  Menu:");
  Serial.println("   [n]umber of stops (" + String(numStops) + ")");
  Serial.println("   [w]ait time at stops (" + String(waitTime) + ")");
  Serial.println("   [t]ime between stops (" + String(travelTime) + ")");
  Serial.println("   E[x]it");
}

String getUserReply(){  
  String inString;
  while (!(Serial.available())){
    delay(10); // wait for data from the user... 
  }

  inString = Serial.readStringUntil('\n');
  inString.trim();
  //Serial.print("   You entered: ");
  Serial.println(inString);
  return inString;
}

void configurationMenu(){
  
    bool changeConfig = false;
    
    Serial.println("  AP Cycler Configuration");
    Serial.print(  "  Change? y/[n] (You have 5 sec to decide) ");
    
    unsigned long t1 = millis();
    unsigned long t2 = t1;

    while (
            !(Serial.available()) && 
            ((t2-t1)<5000)
          )
    {
      t2 = millis();
      delay(500); // wait for data from the user... 
      Serial.print(".");
    }
    
    Serial.println();
    Serial.println();

    if (Serial.available()){
      String ynString = Serial.readStringUntil('\n');
      ynString.trim();
      if (ynString == "y") {changeConfig = true;}
    }

  String ret; 
  if (changeConfig){
    do{
     showMenu();
     ret = getUserReply();
     if (ret == "n"){
       Serial.print("Enter number of stops: ");
       ret = getUserReply();
       numStops = ret.toInt();
     }
     if (ret == "w"){
       Serial.print("Enter wait time at stops: ");
       ret = getUserReply();
       waitTime = ret.toInt();
     }
     if (ret == "t"){
       Serial.print("Enter travel time between stops: ");
       ret = getUserReply();
       travelTime = ret.toInt();
     }
    } while(ret != "x"); 
  }
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
