#pragma once 
#include "ClockDate.h"
#define TIME_BUFFER_SIZE 12

class ClockTime{
public:  
    // ======================================initialization====================================================
    ClockTime():_hour(0), _minute(0), _second(0)
    {

    }

    explicit ClockTime(long timestamp)
    {
      this->_hour = (timestamp / 3600) % 24;
      this->_minute = (timestamp % 3600) / 60;
      this->_second = timestamp % 60; 
    }

    explicit ClockTime(int _hour, int _minute, int _second)
    {
      this->_hour = _hour; 
      this->_minute = _minute;
      this->_second = _second;
    }

    explicit ClockTime(const char* data)
    {
      int intTime[TIME_BUFFER_SIZE];
      //format DD.MM.YEAR HH:MM
      decodeDateTime(data, intTime);
      this->_hour = intTime[3]; 
      this->_minute = intTime[4];
      this->_second = 0;
      //format DD:MM:YEAR
      _toDate = ClockDate(intTime[0], intTime[1], intTime[2]);
    }

    // ======================================interface====================================================
    int getHour() const 
    {
      return _hour; 
    }
    
    int getMinute() const
    {
      return _minute;
    }
    
    int getSecond() const 
    {
      return _second;
    }

    ClockDate getDate() const 
    {
      return _toDate;
    }
    //-----------------------------------------------------------------------------------------------------

    ClockTime addMinute(int _minute)
    {
      int resMinute = _minute + this->_minute;  
      int _hour_carry = resMinute / 60;
      resMinute = resMinute % 60; 
      int resHour = (this->_hour + _hour_carry) % 24; 
      return ClockTime(resHour, resMinute, this->_second);
    }

    ClockTime offset()
    {
         //button clock sometimes take long time to set up the clock 
        //we need to calculate offset for it including _second as well 
        // delay for button: power (4) + memory (4) + _minute (0.1) + _hour (0.1) + finish (0.1)
        // clock offset is 40 sec
        _second += 4 + 4  + 0.15 * _minute  + 0.15 * _hour + 0.15 + 40;

        // compute carry for _minute and _hour from currentSecond 
        int _minute_carry = _second / 60;
        _minute += _minute_carry;
        int _hour_carry = _minute / 60;
        _hour += _hour_carry;
        return *this;
    }

    void incrMinute()
    {
        this->_minute += 1;
        this->_hour += (this->_minute >= 60 ? 1 : 0);
        this->_minute = this->_minute % 60;
        this->_hour = this->_hour % 24;
        // increase day counter after mid night 
        if(isMidNight())
          _toDate.next_day(); 
    }

    bool isMidNight()
    {
        return _hour == 0 && _minute == 0;
    }

    bool operator == (const ClockTime& other) const
    {
        return (this->_hour == other._hour) && (this->_minute == other._minute);
    }

    bool operator < (const ClockTime& other) const 
    {
        auto currClockTimeInMin = _hour * 60 + _minute;
        auto otherClockTimeInMin = other._hour * 60 + other._minute;
        return currClockTimeInMin < otherClockTimeInMin; 
    }

      bool operator > (const ClockTime& other) const 
    {
        auto currClockTimeInMin = _hour * 60 + _minute;
        auto otherClockTimeInMin = other._hour * 60 + other._minute;
        return currClockTimeInMin > otherClockTimeInMin; 
    }

private:
  int _hour, _minute, _second;
  ClockDate _toDate;

protected:

  void decodeDateTime(const char *data, int *timestamp)
  {
      // data = "21.02.2023 17:52";
      // skip unnecessary characters and convert chars to integers
      int i, j;
      int buffer[TIME_BUFFER_SIZE];
      
      i = j = 0;
      while(j < TIME_BUFFER_SIZE)
      {
          int value = data[i] - '0';
          // integer digit cannot be more than 9
          if(value >=0 && value <10)
          {
              buffer[j++] = value;
          }
          i++; 
      }
      
      // 210220231752
  
      timestamp[0] = buffer[0] * 10 + buffer[1]; // day
      timestamp[1] = buffer[2] * 10 + buffer[3]; // month
      timestamp[2] = buffer[4] * 1000 + buffer[5] * 100 + buffer[6] * 10 + buffer[7]; // year
      timestamp[3] = buffer[8] * 10 + buffer[9]; // hour
      timestamp[4] = buffer[10] * 10 + buffer[11]; // minute
  }



};