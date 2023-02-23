#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoOTA.h>
#include "interface/ClockInterface.h"
#include "wifiCredential.h"

#define HOST_NAME "192.168.1.161"
#define USE_ARDUINO_OTA true

MusicClock *music; 
ClockInterface *interface; 
Timer<3, micros> timer;

bool update_smart_clock(void *argument)
{
  int isUpdateClock = (int) argument;
  if(isUpdateClock == 1)
    interface->update_every_minute();
  else
    interface->fast_mqtt_sync();
  
  return true;
}

void setup() {
  // start serial for debugging purpose only
  Serial.begin(115200);
  delay(1000);
  Serial.print("Connecting to ");

  WiFi.begin(net, password);
  Serial.println(net);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(1000);
  // initialized music clock 
  music = new MusicClock();
  ClockTime currentTime("21.02.2023 15:20");
  interface = new ClockInterface(currentTime, music); 

   // start OTA for software update 
  ArduinoOTA.begin();

  // update internal timer
  int isUpdateClock = 1;
  int isUpdateTimer = 0; 
  timer.every(6e7, update_smart_clock, (void *) isUpdateClock);
  timer.every(1e7, update_smart_clock, (void *) isUpdateTimer);  
}

void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
  interface->setMusicState(music->musicLoop());
  interface->mqttLoop();
  timer.tick();
  
}