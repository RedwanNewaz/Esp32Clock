#pragma once 
static const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int FEBRUARY = 2;

class ClockDate{
  
public:  
  ClockDate():_day(22),_month(2),_year(2023){}

  explicit ClockDate(int day, int month, int year)
  {
    this->_day = day;
    this->_month = month;
    this->_year = year; 
  }
  // ======================================interface====================================================
  int getDay() const
  {
    return _day;
  }
  int getMonth() const
  {
    return _month;
  }
  int getYear() const 
  {
    return _year;
  }
  bool isDaylightSaving() const 
  {
    return (*this >= startDaylightSaving() && *this <= endDaylightSaving());
  }

  bool isLeapYear() const 
  {
    return (_year % 400 == 0) || (_year % 4 == 0); 
  }
  int numOfDaysIndex() const
  {
    // prayer time is indexed by number of days 0-364
    
    int dayInYear = 0;
    for (int j = 0; j < _month - 1; ++j) {
      dayInYear += daysInMonth[j];
    }
    dayInYear += _day - 1;
    
    return (_month > FEBRUARY && isLeapYear())? ++dayInYear : dayInYear;
  }
  //-----------------------------------------------------------------------------------------------------

  void next_day()
  {
    _day += 1; 
    if (isLeapYear() && _month == FEBRUARY && _day == 28)
      return;
    else if (_day > daysInMonth[_month-1]) {
        // reset day and incrase month 
        _day = 1;
        _month += 1;
      
        if (_month > 12) {
            // reset month and increase day
            _month = 1;
            _year += 1;
        }
      }
  }

  bool operator >= (const ClockDate& other) const 
  {
    if(_month > other._month)
      return true;
    else if(_month == other._month && _day >= other._day)
      return true;
    return false;
  }

  bool operator <= (const ClockDate& other) const
  {
    if(_month < other._month)
      return true;
    else if(_month == other._month && _day <= other._day)
      return true;
    return false;
  }
private:
  int _month, _day, _year;

protected:
  ClockDate startDaylightSaving() const
  {
    // start Sunday, March 13
    return ClockDate(03, 13, _year);
  }
  
  ClockDate endDaylightSaving() const 
  {
    // ends Sunday, November 6
    return ClockDate(06, 11, _year);
  }
};