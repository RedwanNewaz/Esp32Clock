#pragma once
#include "../data/prayer_table.hpp"
#include "ClockDate.h"
#include "ClockTime.h"
#include <time.h>

#define NUM_PRAYERS 5
#define HOURS_PER_DAY 24
#define MILLI_SECOND 1000
#define CST (-6)

class PrayerTime {
public:
  PrayerTime() {
    _totalPrayers = sizeof(prayerTable) / sizeof(long long);
    // printf("total index %d \n", _totalPrayers);
  }
  explicit PrayerTime(const ClockDate &today) {
    _totalPrayers = sizeof(prayerTable) / sizeof(long long);
    updateDate(today);
  }

  void updateDate(const ClockDate &today) {
    // update calander
    int dayInYear = today.numOfDaysIndex();

    for (int i = 0; i < NUM_PRAYERS; ++i) {
      int tableIndex = (dayInYear * NUM_PRAYERS + i) % _totalPrayers;
      long long timestamp = prayerTable[tableIndex] / MILLI_SECOND;
      updateTimetable(timestamp, i, today);
    }
  }

  void get(ClockTime *prayers) {
    for (int i = 0; i < NUM_PRAYERS; ++i) {
      prayers[i] = ClockTime(_timetable[i][0], _timetable[i][1], 0);
    }
  }

private:
  void updateTimetable(long long timestamp, int index, const ClockDate &today) {
    struct tm *timeinfo = gmtime(&timestamp);
    _timetable[index][0] =
        (HOURS_PER_DAY + timeinfo->tm_hour + CST + today.isDaylightSaving()) %
        HOURS_PER_DAY;
    _timetable[index][1] = timeinfo->tm_min;
  }
  int _timetable[NUM_PRAYERS][2];
  int _totalPrayers;
};