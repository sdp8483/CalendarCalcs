#ifndef INC_CALENDARCALCS_H
#define INC_CALENDARCALCS_H

/* Includes ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdio.h>

/* Defines ------------------------------------------------------------------- */
#ifndef CALENDARCALCS_LOG_MESSAGES
#define CALENDARCALCS_LOG_MESSAGES      true
#endif

#define CALENDARCALCS_ERROR             -1    // return value if there is an error

/* Macros -------------------------------------------------------------------- */
#if CALENDARCALCS_LOG_MESSAGES
#define calcalc_log(__info,...)         printf("CalendarCalcs: " __info,##__VA_ARGS__)
#else
#define calcalc_log(__info,...)
#endif

/* typedef ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------- */

/* External Globals ---------------------------------------------------------- */

/* Function Prototypes ------------------------------------------------------- */

/* Class --------------------------------------------------------------------- */
class CalendarCalcs {
    public:
        struct datetime {               /* same structure as RP2040 datetime_t */
            int16_t year;
            int8_t month;               /* 1 to 12, 1 is January */
            int8_t day;                 /* 1 to 31, depends on month */
            int8_t day_of_week;         /* 0 to 6, 0 is Sunday */
            int8_t hour;                /* 0 to 23 */
            int8_t minute;              /* 0 to 59 */
            int8_t second;              /* 0 to 59 */
        };

        enum TIMEZONE {
            TZ_ERROR        = CALENDARCALCS_ERROR,
            EST             = 0,        // eastern standard time
            MST             = 1,        // mountain standard time
            MST_AZ          = 2,        // Arizona specific mountain standard time
            CST             = 3,        // central standard time
            PST             = 4,        // pacific standard time
            AKST            = 5,        // alaskan standard time
            HST             = 6,        // hawaiian standard time
        };

        enum DAY_OF_WEEK {
            DOW_ERROR       = -4,
            DOW_ERROR_DAY   = -3,
            DOW_ERROR_MONTH = -2,
            DOW_ERROR_YEAR  = - 1,
            SUNDAY          = 0,
            MONDAY          = 1,
            TUESDAY         = 2,
            WEDNESDAY       = 3,
            THURSDAY        = 4,
            FRIDAY          = 5,
            SATURDAY        = 6
        };

        enum MONTH {
            JANUARY         = 1,
            FEBRUARY        = 2,
            MARCH           = 3,
            APRIL           = 4,
            MAY             = 5,
            JUNE            = 6,
            JULY            = 7,
            AUGUST          = 8,
            SEPTEMBER       = 9,
            OCTOBER         = 10,
            NOVEMBER        = 11,
            DECEMBER        = 12
        };

        enum DST {
            DST_ERROR       = -4,
            DST_ERROR_DAY   = -3,
            DST_ERROR_MONTH = -2,
            DST_ERROR_YEAR  = - 1,
            INACTIVE        = 0,
            ACTIVE          = 1,
        };

        enum CalendarCalcs_Error {
            NONE,
            DATE_INVALID,
            TIME_INVALID,
        };

        bool is_leap_year(uint16_t year);
        CalendarCalcs::DAY_OF_WEEK day_of_week(uint16_t year, int8_t month, int8_t day);
        CalendarCalcs::DST is_daylight_savings(CalendarCalcs::TIMEZONE tz, uint16_t year, 
                                               int8_t month, int8_t day, int8_t utc_hour);
        
        bool date_is_valid(uint16_t year, int8_t month, int8_t day);
        bool time_is_valid(int8_t hour, int8_t minute, int8_t second);

        CalendarCalcs::CalendarCalcs_Error to_local_time(CalendarCalcs::TIMEZONE tz, CalendarCalcs::datetime *dt);

    private:
        // bool _year_is_valid(int16_t year);
        bool _month_is_valid(int8_t month);
        int8_t _days_in_month(uint16_t year, int8_t month);
        bool _day_is_valid(uint16_t year, int8_t month, int8_t day);

        int8_t _utc_offset_standard(CalendarCalcs::TIMEZONE tz);

        void _increment_day_of_week(CalendarCalcs::DAY_OF_WEEK *dow);

        int8_t _ordinal_day_of_month(uint8_t nth, CalendarCalcs::DAY_OF_WEEK nth_dow, 
                                     int16_t year, int8_t month);     // ie get the 2nd Sunday of March

        // used in day of the week calculations
        const int8_t _month_key[13] = {0, 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
        const int8_t _century_key[4] = {6, 4, 2, 0};

        // UTC offset by timezone for standard time
        const int8_t _est_utc_offset    = -5;
        const int8_t _mst_utc_offset    = -7;
        const int8_t _mst_az_utc_offset = _mst_utc_offset;
        const int8_t _cst_utc_offset    = -6;
        const int8_t _pst_utc_offset    = -8;
        const int8_t _akst_utc_offset   = -9;
        const int8_t _hst_utc_offset    = -10;
};
#endif