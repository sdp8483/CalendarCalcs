#include "CalendarCalcs.h"
#include <cstdlib>

CalendarCalcs ccalcs;

/* Perform Leap Year Test on specified year */
void LeapYearTest(uint16_t year, bool expected_result) {
    if (expected_result != ccalcs.is_leap_year(year)) {
        std::exit(1);
    }
}

/* Perform Day of the Week Test on specified date */
void DayOfWeekTest(uint16_t year, uint8_t month, uint8_t day, CalendarCalcs::DAY_OF_WEEK expected_result) {
    if (expected_result != ccalcs.day_of_week(year, month, day)) {
        std::exit(1);
    }
}

/* Perform a Daylight Savings Test on specified date */
void DaylightSavingsTest(CalendarCalcs::TIMEZONE tz, uint16_t year, 
                         uint8_t month, uint8_t day, uint8_t utc_hour,
                         CalendarCalcs::DST expected_result) {
    if (expected_result != ccalcs.is_daylight_savings(tz, year, month, day, utc_hour)) {
        std::exit(1);
    }
}