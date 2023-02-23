#pragma once
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "../clock/ClockTime.h"
#include "ButtonInterface.h"
// subscribe topic 
static const char* hourTopic = "/clock/alarm/set/hour";
static const char* minuteTopic = "/clock/alarm/set/minute";
static const char* startTopic = "/clock/alarm/start";
static const char* resetTopic = "/clock/reset";
static const char* presetTopic = "/clock/alarm/preset";
static const char* syncTopic = "/clock/time/sync";

//publish topic 
static const char* pubTime = "/clock/time";
static const char* pubAlarm = "/clock/alarm";
static const char* pubPrayerTime = "/clock/prayer/time";
static const char* pubPrayerName = "/clock/prayer/name";


class MqttInterface:public ButtonInterface{
public:
    MqttInterface();
    void mqttLoop();
    void init();
    void publishTime(const ClockTime& time, const ClockTime& prayerTime, const ClockTime& alarmTime, const char* prayerName);
    
protected:
    /// reset(0) reinitialize base::clock 
    /// reset(1) reinitilize button::clock only
    /// use reset(1) when internet (mqtt) time is accurate but clock display wrong time
    /// when mqtt time is also wrong then use any number to reset the base clock and button clock
    virtual void reset(int type) = 0;
    virtual void presetAlarm(int minute) = 0;
    virtual void reset(const ClockTime& current) = 0;
    virtual void setAlarmTime(const ClockTime &alarm) = 0;

private:
    void callback(char* topic, byte* payload, unsigned int length);
    void reconnect();
    String paddedStr(int val) const; 
    String toString(const ClockTime& time) const;

        
private:
    WiFiClient wifiClient_;
    IPAddress *server_;
    PubSubClient *client_;
    int alarm_time_[2];
};