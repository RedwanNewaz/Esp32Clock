#pragma once

#include "../clock/ClockState.h"
#include "../clock/PrayerTime.h"
#include "MusicInterface.h"
#include "MqttInterface.h"

/**
  SMART CLOCK FEATURES
  (i)   internal clock update
  (ii)  automatic button control
  (iii) prayer time alert
  (iv)  preset alarm (e.g., washing, drayer, pizza)
  (v)   sleep time alarm (wake up in the morning)
  (vi)  mqtt inteface to communicate with cell phone
**/

class ClockInterface : public MqttInterface{
public:
  ClockInterface(const ClockTime &time, MusicClock *music) : _currentTime(time) {
    updatePrayers();
  }

  void reset(const ClockTime &time)override {
    _currentTime = time;
    updatePrayers();
    setTime(_currentTime.getHour(), _currentTime.getMinute());
  }

  void update_every_minute() {
    // update internal timer
    _currentTime.incrMinute();
    
    // is it a New day? then update prayer times
    if (_currentTime.isMidNight())
      updatePrayers();
  }

  void fast_mqtt_sync()
  {
    // check activities for music clock
    char *prayerNames[] = {"Fajr", "Dhuhr", "Asr", "Maghrib", "Isha"};
    bool azanActive = false;
    for (int i = 0; i < NUM_PRAYERS; ++i) {
      // publish mqtt prayer time message
      //  printf("%s = %02d:%02d \n", prayerNames[i], _prayerTimes[i].getHour(),
      publishTime(_currentTime, _prayerTimes[i], _alarmTime, prayerNames[i]);
      //  _prayerTimes[i].getMinute());
      azanActive = (azanActive || (_currentTime == _prayerTimes[i]));
    }
    _stateMachine.setAzan(azanActive);
    if (_stateMachine.playAzan()) {
      // activate azan sound
      _music->begin(Azan);
      // printf("playing azan sound \n");
    } else if (_stateMachine.playAlarm()) {
      // activate alarm sound
      _music->begin(Alarm);
      // reset alarm time
      _alarmTime = ClockTime();
    }
  }

  void presetAlarm(int minute) override
  {
    setAlarmTime(_currentTime.addMinute(minute));
  }



  void setAlarmTime(const ClockTime &alarm) override{
    _alarmTime = alarm;
    _stateMachine.setAlarm(true);
  }

  void setMusicState(bool state)
  {
    _stateMachine.setMusic(state);
  }

  void cancelAlarmTime() {
    _alarmTime = ClockTime();
    _stateMachine.setAlarm(false);
  }

  void reset(int type) override
  {
    if(type == 1)
      setTime(_currentTime.getHour(), _currentTime.getMinute());
    else
      cancelAlarmTime();
  }

private:
  ClockTime _currentTime;
  ClockTime _alarmTime;
  ClockState _stateMachine;
  ClockTime _prayerTimes[NUM_PRAYERS];
  PrayerTime _prayers;
  MusicClock *_music; 

private:
  void updatePrayers() {
    _prayers.updateDate(_currentTime.getDate());
    _prayers.get(_prayerTimes);
  }
};
