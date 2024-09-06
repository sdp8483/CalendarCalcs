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
void DayOfWeekTest(uint16_t year, int8_t month, int8_t day, CalendarCalcs::DAY_OF_WEEK expected_result) {
    CalendarCalcs::DAY_OF_WEEK dow = ccalcs.day_of_week(year, month, day);

    printf("Calculated day of the week for %d/%d/%d is %d\r\n", month, day, year, dow);

    if (expected_result != dow) {
        std::exit(1);
    }
}

/* Perform a Daylight Savings Test on specified date */
void DaylightSavingsTest(CalendarCalcs::TIMEZONE tz, uint16_t year, 
                         int8_t month, int8_t day, int8_t utc_hour,
                         CalendarCalcs::DST expected_result) {
    if (expected_result != ccalcs.is_daylight_savings(tz, year, month, day, utc_hour)) {
        std::exit(1);
    }
}

/* Perform date validation test on specified date */
void DateIsValidTest(uint16_t year, int8_t month, int8_t day, bool expected_result) {
    if (expected_result != ccalcs.date_is_valid(year, month, day)) {
        std::exit(1);
    }
}

/* Perform time validation test on specified time */
void TimeIsValidTest(int8_t hour, int8_t minute, int8_t second, bool expected_result) {
    if (expected_result != ccalcs.time_is_valid(hour, minute, second)) {
        std::exit(1);
    }
}

/* Perform Local Time Validation Test on specified time and timezone */
void LocalTimeTest(CalendarCalcs::TIMEZONE tz,
                   CalendarCalcs::datetime *utc_time, 
                   CalendarCalcs::datetime *expected_result) {
    
    ccalcs.to_local_time(tz, utc_time);

    if (expected_result->year != utc_time->year) {
        printf("expected year %d != %d\r\n", expected_result->year, utc_time->year);
        std::exit(1);
    }

    if (expected_result->month != utc_time->month) {
        printf("expected month %d != %d\r\n", expected_result->month, utc_time->month);
        std::exit(1);
    }

    if (expected_result->day != utc_time->day) {
        printf("expected day %d != %d\r\n", expected_result->day, utc_time->day);
        std::exit(1);
    }

    if (expected_result->hour != utc_time->hour) {
        printf("expected hour %d != %d\r\n", expected_result->hour, utc_time->hour);
        std::exit(1);
    }

    if (expected_result->minute != utc_time->minute) {
        printf("expected minute %d != %d\r\n", expected_result->minute, utc_time->minute);
        std::exit(1);
    }

    if (expected_result->second != utc_time->second) {
        printf("expected second %d != %d\r\n", expected_result->second, utc_time->second);
        std::exit(1);
    }

    if (expected_result->day_of_week != utc_time->day_of_week) {
        printf("expected day of week %d != %d\r\n", expected_result->day_of_week, utc_time->day_of_week);
        std::exit(1);
    }
}