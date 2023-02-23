#pragma once 


class  ClockState{
public:
  ClockState()
  {
    _musicState = IDLE; 
    _prayerState = INACTIVE;
    _alarmState = UNSET; 
  }

  void setAlarm(bool cmd)
  {
    // cmd false -> UNSET and true -> SET
    // EXECUTING status can only set from playAlarm function
    _alarmState = static_cast<ALARM_STATE>(cmd);
  }

  void setAzan(bool cmd)
  {
    // set true when current time matches with any azan time 
    _prayerState = static_cast<PRAYER_STATE>(cmd);
  }


  void setMusic(bool cmd)
  {
    _musicState = static_cast<MUSIC_STATE>(cmd);
  }

  bool playAzan()
  {
    if(_musicState == IDLE && _prayerState == ACTIVE)
    {
      setMusic(true);
      return true;
    }
    else
      _prayerState = INACTIVE;
    
    return false;
  }

  bool playAlarm()
  {
    // play alarm when user set a value
    if(_musicState == IDLE && _alarmState == SET)
    {
      _alarmState = EXECUTING;
      setMusic(true);
      return true;
    }
    // when music clock finishes playing alarm unset alarm status again  
    else if (_alarmState == EXECUTING && _musicState == IDLE)
      _alarmState = UNSET;
    return false;
  }

private:
  enum MUSIC_STATE
  {
    IDLE, 
    RUNNING
  }_musicState;

  enum PRAYER_STATE
  {
    INACTIVE,    
    ACTIVE   
  }_prayerState; 

  enum ALARM_STATE
  {
    UNSET,
    SET, 
    EXECUTING
  }_alarmState;   
  
};
