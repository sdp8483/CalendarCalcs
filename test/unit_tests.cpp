/* Includes ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdio.h>
// #include <string.h>
#include <string> 

#include "CalendarCalcs.h"

/* Defines ------------------------------------------------------------------- */

/* Macros -------------------------------------------------------------------- */

/* typedef ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------- */

/* External Globals ---------------------------------------------------------- */

/* Function Prototypes ------------------------------------------------------- */
void LeapYearTest(uint16_t year, bool expected_result);
void DayOfWeekTest(uint16_t year, int8_t month, int8_t day, CalendarCalcs::DAY_OF_WEEK expected_result);
void DaylightSavingsTest(CalendarCalcs::TIMEZONE tz, uint16_t year, 
                         int8_t month, int8_t day, int8_t utc_hour,
                         CalendarCalcs::DST expected_result);
void DateIsValidTest(uint16_t year, int8_t month, int8_t day, bool expected_result);
void TimeIsValidTest(int8_t hour, int8_t minute, int8_t second, bool expected_result);
void LocalTimeTest(CalendarCalcs::TIMEZONE tz,
                   CalendarCalcs::datetime *utc_time, 
                   CalendarCalcs::datetime *expected_result);

int main(int argc, char *argv[]) {
    /* Leap Year Tests ------------------------------------------------------- */
    /* Leap Year Test for 2024 which is a leap year since it is divisible by 4 */
    if (argc < 2 || argv[1] == std::string("1")) {
        LeapYearTest(2024, true);
    }

    /* Leap Year Test for 2025 which is not a leap year since it is not divisible by 4 */
    if (argc < 2 || argv[1] == std::string("2")) {
        LeapYearTest(2025, false);
    }

    /* Leap Year Test for 2100 which is not a leap year since it is divisible by 100 */
    if (argc < 2 || argv[1] == std::string("3")) {
        LeapYearTest(2025, false);
    }

    /* Leap Year Test for 2000 which is a leap year since it is divisible by 400 */
    if (argc < 2 || argv[1] == std::string("4")) {
        LeapYearTest(2000, true);
    }

    /* Day of the Week Tests ------------------------------------------------- */
    /* February 29, 2024 is a Thursday and a Leap Day */
    if (argc < 2 || argv[1] == std::string("5")) {
        DayOfWeekTest(2024, 2, 29, CalendarCalcs::DAY_OF_WEEK::THURSDAY);
    }

    /* August 26, 2024 is a Monday */
    if (argc < 2 || argv[1] == std::string("6")) {
        DayOfWeekTest(2024, 8, 26, CalendarCalcs::DAY_OF_WEEK::MONDAY);
    }

    /* April 10, 1878 is a Wednesday */
    if (argc < 2 || argv[1] == std::string("7")) {
        DayOfWeekTest(1878, 4, 10, CalendarCalcs::DAY_OF_WEEK::WEDNESDAY);
    }

    /* July 22, 2829 is a Sunday */
    if (argc < 2 || argv[1] == std::string("8")) {
        DayOfWeekTest(2829, 7, 22, CalendarCalcs::DAY_OF_WEEK::SUNDAY);
    }

    /* There is no month 13 */
    if (argc < 2 || argv[1] == std::string("9")) {
        DayOfWeekTest(2024, 13, 26, CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_MONTH);
    }

    /* There is no month 0 */
    if (argc < 2 || argv[1] == std::string("10")) {
        DayOfWeekTest(2024, 0, 26, CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_MONTH);
    }

    /* There are not 31 days in September */
    if (argc < 2 || argv[1] == std::string("11")) {
        DayOfWeekTest(2024, 9, 31, CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_DAY);
    }

    /* February 29th 2023 did not exist */
    if (argc < 2 || argv[1] == std::string("12")) {
        DayOfWeekTest(2023, 2, 29, CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_DAY);
    }

    /* Daylight Savings Time Tests ------------------------------------------- */
    /* DST Starts March 10, 2024 2am Local Time */
    if (argc < 2 || argv[1] == std::string("13")) {
        DaylightSavingsTest(CalendarCalcs::TIMEZONE::EST, 2024, 3, 10, 6, CalendarCalcs::DST::INACTIVE);
    }

    if (argc < 2 || argv[1] == std::string("14")) {
        DaylightSavingsTest(CalendarCalcs::TIMEZONE::EST, 2024, 3, 10, 7, CalendarCalcs::DST::ACTIVE);
    }

    /* DST Ends November 3, 2024 2am Local Time */
    if (argc < 2 || argv[1] == std::string("15")) {
        DaylightSavingsTest(CalendarCalcs::TIMEZONE::EST, 2024, 11, 3, 6, CalendarCalcs::DST::ACTIVE);
    }

    if (argc < 2 || argv[1] == std::string("16")) {
        DaylightSavingsTest(CalendarCalcs::TIMEZONE::EST, 2024, 11, 3, 7, CalendarCalcs::DST::INACTIVE);
    }

    /* Valid Date Tests ------------------------------------------------------ */
    /* September 3rd, 2024 is a valid date */
    if (argc < 2 || argv[1] == std::string("17")) {
        DateIsValidTest(2024, 9, 3, true);
    }

    /* No month greater then 12 */
    if (argc < 2 || argv[1] == std::string("18")) {
        DateIsValidTest(2024, 15, 21, false);
    }

    /* September does not have 31 days */
    if (argc < 2 || argv[1] == std::string("19")) {
        DateIsValidTest(2024, 9, 31, false);
    }

    /* Valid Time Tests ------------------------------------------------------ */
    /* 8:14:54 is a valid test */
    if (argc < 2 || argv[1] == std::string("20")) {
        TimeIsValidTest(8, 14, 54, true);
    }

    /* no hour >= 24 */
    if (argc < 2 || argv[1] == std::string("21")) {
        TimeIsValidTest(24, 14, 54, false);
    }

    /* no minute >= 60 */
    if (argc < 2 || argv[1] == std::string("22")) {
        TimeIsValidTest(8, 60, 54, false);
    }

    /* no second >= 60 */
    if (argc < 2 || argv[1] == std::string("23")) {
        TimeIsValidTest(8, 14, 60, false);
    }

    /* Local Time Conversions ------------------------------------------------ */
    /* September 5, 2024 17:40:37 to EDT */
    if (argc < 2 || argv[1] == std::string("24")) {
        CalendarCalcs::datetime utc_1 = {
            .year = 2024,
            .month = 9,
            .day = 5,
            .hour = 17,
            .minute = 40,
            .second = 37,
            .day_of_week = -1
        };

        CalendarCalcs::datetime edt_1 = {
            .year = 2024,
            .month = 9,
            .day = 5,
            .hour = 13,
            .minute = 40,
            .second = 37,
            .day_of_week = CalendarCalcs::DAY_OF_WEEK::THURSDAY
        };

        LocalTimeTest(CalendarCalcs::TIMEZONE::EST, &utc_1, &edt_1);
    }

    /* September 1, 2024 3:40:37 to EDT, roll back month */
    if (argc < 2 || argv[1] == std::string("25")) {
        CalendarCalcs::datetime utc_2 = {
            .year = 2024,
            .month = 9,
            .day = 1,
            .hour = 3,
            .minute = 40,
            .second = 37,
            .day_of_week = -1
        };

        CalendarCalcs::datetime edt_2 = {
            .year = 2024,
            .month = 8,
            .day = 31,
            .hour = 23,
            .minute = 40,
            .second = 37,
            .day_of_week = CalendarCalcs::DAY_OF_WEEK::SATURDAY
        };

        LocalTimeTest(CalendarCalcs::TIMEZONE::EST, &utc_2, &edt_2);
    }

    /* January 1, 2025 3:40:37 to EDT, roll back year */
    if (argc < 2 || argv[1] == std::string("26")) {
        CalendarCalcs::datetime utc_3 = {
            .year = 2025,
            .month = 1,
            .day = 1,
            .hour = 3,
            .minute = 40,
            .second = 37,
            .day_of_week = -1
        };

        CalendarCalcs::datetime edt_3 = {
            .year = 2024,
            .month = 12,
            .day = 31,
            .hour = 22,
            .minute = 40,
            .second = 37,
            .day_of_week = CalendarCalcs::DAY_OF_WEEK::TUESDAY
        };

        LocalTimeTest(CalendarCalcs::TIMEZONE::EST, &utc_3, &edt_3);
    }
}