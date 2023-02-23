#include "interface/MqttInterface.h"


MqttInterface::MqttInterface()
{
    client_ = new PubSubClient(wifiClient_);
    server_ = new IPAddress(192, 168, 1, 1);
    alarm_time_[0] = alarm_time_[1] = 0;
}

void MqttInterface::init()
{
    client_->setServer(*server_, 1883);
    client_->setCallback([=](char* topic, byte* payload, unsigned int length){this->callback(topic, payload, length);});
}

void MqttInterface::callback(char* topic, byte* payload, unsigned int length)
{

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String data = "";
    for (int i = 0; i < length; i++) {
      data += (char)payload[i];
    }
    Serial.println(data);

    // set values 
    if(String(topic).equals(hourTopic))
      alarm_time_[0] = data.toInt();
    else if(String(topic).equals(minuteTopic))
      alarm_time_[1] = data.toInt();
    else if(String(topic).equals(startTopic))
    {
        ClockTime alarm(alarm_time_[0], alarm_time_[1], 0);
        setAlarmTime(alarm);
    }
    else if(String(topic).equals(resetTopic))
      reset(data.toInt());
    else if(String(topic).equals(presetTopic))
      presetAlarm(data.toInt());
    else if(String(topic).equals(syncTopic))
      {
        ClockTime current(data.c_str());
        reset(current);
      }
 
     
 
}

void MqttInterface::reconnect()
{
    // Loop until we're reconnected
    while (!client_->connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client_->connect("arduinoClient")) {
        Serial.println("Connected!!");
        // ... and resubscribe
        client_->subscribe(hourTopic);
        client_->subscribe(minuteTopic);
        client_->subscribe(startTopic);
        client_->subscribe(resetTopic);
        client_->subscribe(presetTopic);
        client_->subscribe(syncTopic);
      } 
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(client_->state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
}


void MqttInterface::mqttLoop()
{
    if (!client_->connected()) {
      reconnect();
    }
    client_->loop();
}

String MqttInterface::paddedStr(int val) const 
{
    String str = String(val);
    if(str.length() == 1)
        str = "0" + str;
    return str; 
}

String MqttInterface::toString(const ClockTime& time) const
{
    String format = paddedStr(time.getHour()) + ":" + paddedStr(time.getMinute());
    return format;
}



void MqttInterface::publishTime(const ClockTime& time, const ClockTime& prayerTime, const ClockTime& alarmTime, const char* prayerName)
{
    client_->publish(pubTime, toString(time).c_str());
    client_->publish(pubAlarm, toString(alarmTime).c_str());
    client_->publish(pubPrayerTime, toString(prayerTime).c_str());
    client_->publish(pubPrayerName, prayerName);
}