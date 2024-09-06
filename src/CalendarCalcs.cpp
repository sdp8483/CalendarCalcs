#include "CalendarCalcs.h"

/* return true if it is a leap year ------------------------------------------ */
/* https://learn.microsoft.com/en-us/office/troubleshoot/excel/determine-a-leap-year */
bool CalendarCalcs::is_leap_year(uint16_t year) {
    /* assume it is not a leap year */
    bool leap_year = false;

    /* if the year is divisible by 4 it is a leap year */
    if ((year % 4) == 0) {
        leap_year = true;
    }

    /* but if the year is divisible by 100 it is not a leap year */
    if ((year % 100) == 0) {
        leap_year = false;
    }

    /* except when the year is divisible by 400 */
    if ((year % 400) == 0) {
        leap_year = true;
    }

    return leap_year;
}

/* return a number indicating the day of the week given a date --------------- */
/* https://www.almanac.com/how-find-day-week this formula seems to only work for 2000 - 2099 */
/* https://artofmemory.com/blog/how-to-calculate-the-day-of-the-week/ */
CalendarCalcs::DAY_OF_WEEK CalendarCalcs::day_of_week(uint16_t year, int8_t month, int8_t day) {
    calcalc_log("DOW received the following date: %d/%d/%d\r\n", month, day, year);

    // check validity of month
    if (!_month_is_valid(month)) {
        return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_MONTH;
    }

    // check validity of day
    if (!_day_is_valid(year, month, day)) {
        return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_DAY;
    }

    // calculate year code = (YY + (YY / 4)) mod 7
    uint16_t yy = year % 100;                   // get last two digits of the year
    uint16_t year_code = (yy + (yy / 4)) % 7;   // sum yy and quotient of yy/4 then mod 7

    // find month code
    uint16_t month_code = _month_key[month];

    // find century code, it is a repeating pattern starring at century 0 of 4 numbers
    uint8_t century = year / 100;
    int8_t century_code = _century_key[century % 4];

    // calculate sum
    uint16_t sum = year_code + month_code + century_code + day;

    // adjust for leap years if month is jan or feb
    if (is_leap_year(year) && (month <= CalendarCalcs::MONTH::FEBRUARY)) {
        sum -= 1;
    }

    uint8_t dow = sum % 7;

    switch (dow) {
        case 0:
            return CalendarCalcs::DAY_OF_WEEK::SUNDAY;
            break;
    
        case 1:
            return CalendarCalcs::DAY_OF_WEEK::MONDAY;
            break;
        
        case 2:
            return CalendarCalcs::DAY_OF_WEEK::TUESDAY;
            break;
    
        case 3:
            return CalendarCalcs::DAY_OF_WEEK::WEDNESDAY;
            break;

        case 4:
            return CalendarCalcs::DAY_OF_WEEK::THURSDAY;
            break;
    
        case 5:
            return CalendarCalcs::DAY_OF_WEEK::FRIDAY;
            break;
        
        case 6:
            return CalendarCalcs::DAY_OF_WEEK::SATURDAY;
            break;

        default:
            return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR;
            break;
    }
}

/* determine if it is daylight savings time ---------------------------------- */
/* https://www.nist.gov/pml/time-and-frequency-division/popular-links/daylight-savings-time-dst */
CalendarCalcs::DST CalendarCalcs::is_daylight_savings(CalendarCalcs::TIMEZONE tz, uint16_t year, int8_t month, int8_t day, int8_t utc_hour) {
    // calcalc_log("DST Check received the following data: %d/%d/%d, %d UTC, Timezone: %d\r\n",
    //             month, day, year, utc_hour, tz);

    if (!_month_is_valid(month)) {
        return CalendarCalcs::DST::DST_ERROR_MONTH;
    }

    if (!_day_is_valid(year, month, day)) {
        return CalendarCalcs::DST::DST_ERROR_DAY;
    }

    // Arizona does not observe daylight savings time
    if (tz == CalendarCalcs::TIMEZONE::MST_AZ) {
        return CalendarCalcs::DST::INACTIVE;
    }

    // Hawaiian time zone does not observe daylight savings time
    if (tz == CalendarCalcs::TIMEZONE::HST) {
        return CalendarCalcs::DST::INACTIVE;
    }

    // daylight savings time starts at 2am 2nd Sunday in March
    int8_t second_sunday_in_march = _ordinal_day_of_month(2, CalendarCalcs::DAY_OF_WEEK::SUNDAY, year, CalendarCalcs::MONTH::MARCH);
    // calcalc_log("Second Sunday in March %d is %d\r\n", year, second_sunday_in_march);
    // daylight savings time ends at 2am 1st Sunday in November
    int8_t first_sunday_in_november = _ordinal_day_of_month(1, CalendarCalcs::DAY_OF_WEEK::SUNDAY, year, CalendarCalcs::MONTH::NOVEMBER);
    // calcalc_log("First Sunday in November %d is %d\r\n", year, first_sunday_in_november);

    // get the local hour in standard time
    int8_t local_hour_stdTime = utc_hour + _utc_offset_standard(tz);
    // calcalc_log("Local hour is %d\r\n", local_hour_stdTime);

    switch (month) {
        case CalendarCalcs::MONTH::JANUARY:
            return CalendarCalcs::DST::INACTIVE;
            break;

        case CalendarCalcs::MONTH::FEBRUARY:
            return CalendarCalcs::DST::INACTIVE;
            break;

        // DST starts Second Sunday in March at 2am local time
        case CalendarCalcs::MONTH::MARCH:
            if (day < second_sunday_in_march) {
                return CalendarCalcs::DST::INACTIVE;
            } else if (day > second_sunday_in_march) {
                return CalendarCalcs::DST::ACTIVE;
            }

            // day is the second Sunday in March after 2am local time
            if (local_hour_stdTime >= 2) {
                return CalendarCalcs::DST::ACTIVE;
            }
            
            // its the second Sunday in March but before 2am local time
            return CalendarCalcs::DST::INACTIVE;
            break;

        case CalendarCalcs::MONTH::APRIL:
            return CalendarCalcs::DST::ACTIVE;
            break;

        case CalendarCalcs::MONTH::MAY:
            return CalendarCalcs::DST::ACTIVE;
            break;

        case CalendarCalcs::MONTH::JUNE:
            return CalendarCalcs::DST::ACTIVE;
            break;

        case CalendarCalcs::MONTH::JULY:
            return CalendarCalcs::DST::ACTIVE;
            break;

        case CalendarCalcs::MONTH::AUGUST:
            return CalendarCalcs::DST::ACTIVE;
            break;

        case CalendarCalcs::MONTH::SEPTEMBER:
            return CalendarCalcs::DST::ACTIVE;
            break;
        case CalendarCalcs::MONTH::OCTOBER:
            return CalendarCalcs::DST::ACTIVE;
            break;

        // DST ends first Sunday in November at 2am local time
        case CalendarCalcs::MONTH::NOVEMBER:
            if (day < first_sunday_in_november) {
                return CalendarCalcs::DST::ACTIVE;
            } else if (day > first_sunday_in_november) {
                return CalendarCalcs::DST::INACTIVE;
            }

            // day is 1st Sunday in November after 2am local time
            if (local_hour_stdTime >= 2) {
                return CalendarCalcs::DST::INACTIVE;
            }
            
            // day is 1st Sunday in November but before 2am local time
            return CalendarCalcs::DST::ACTIVE;
            break;
        case CalendarCalcs::MONTH::DECEMBER:
            return CalendarCalcs::DST::INACTIVE;
            break;

        default:
            // with a validity check on the month this should never be returned
            return CalendarCalcs::DST::ACTIVE;
            break;
    }

    // should never reach here
    return CalendarCalcs::DST::ACTIVE;
}

/* determine if a date is valid ---------------------------------------------- */
bool CalendarCalcs::date_is_valid(uint16_t year, int8_t month, int8_t day) {
    /* is the month valid, not greater then 12 */
    if (!_month_is_valid(month)) {
        return false;
    }

    /* is the number of days correct for the month and year */
    if (!_day_is_valid(year, month, day)) {
        return false;
    }

    return true;
}

/* determine if a time is valid ---------------------------------------------- */
bool CalendarCalcs::time_is_valid(int8_t hour, int8_t minute, int8_t second) {
    /* are there more then 24 hours */
    if (hour >= 24) {
        return false;
    }

    /* are there more then 59 minutes */
    if (minute >= 60) {
        return false;
    }

    /* are there more then 59 seconds */
    if (second >= 60) {
        return false;
    }

    return true;
}

/* convert UTC datetime to local datetime ------------------------------------ */
CalendarCalcs::CalendarCalcs_Error CalendarCalcs::to_local_time(CalendarCalcs::TIMEZONE tz, CalendarCalcs::datetime *dt) {
    calcalc_log("%d/%d/%d %d:%d:%d UTC\r\n", dt->month, dt->day, dt->year, dt->hour, dt->minute, dt->second);
    calcalc_log("Timezone is %d\r\n", tz);

    /* make sure date is valid */
    if (!date_is_valid(dt->year, dt->month, dt->day)) {
        return CalendarCalcs::CalendarCalcs_Error::DATE_INVALID;
    }

    /* make sure time is valid */
    if (!time_is_valid(dt->hour, dt->minute, dt->second)) {
        return CalendarCalcs::CalendarCalcs_Error::TIME_INVALID;
    }

    /* get timezone offset */
    int8_t tz_offset = _utc_offset_standard(tz);

    /* adjust for daylight savings time if in effect, add 1 hour */
    if (is_daylight_savings(tz, dt->year, dt->month, dt->day, dt->hour) == CalendarCalcs::DST::ACTIVE) {
        tz_offset += 1;
    }

    /* apply timezone offset */
    dt->hour += tz_offset;

    /* increment day if hour has passed 24 */
    if (dt->hour >= 24) {
        dt->hour -= 24;
        dt->day++;
    }

    /* decrement day if hour is negative */
    if (dt->hour < 0) {
        dt->hour += 24;
        dt->day--;
    }

    /* increment month if day has passed max days */
    if (dt->day > _days_in_month(dt->year, dt->month)) {
        dt->month++;
        dt->day -= _days_in_month(dt->year, dt->month);
    }

    /* decrement month if day is less then 1 */
    if (dt->day < 1) {
        dt->month--;
        dt->day += _days_in_month(dt->year, dt->month);
    }

    /* increment year if month is greater then 12 */
    if (dt->month > 12) {
        dt->year++;
        dt->month = 1;
        dt->day = 1;
    }

    /* decrement year if month is less then 1 */
    if (dt->month < 1) {
        dt->year--;
        dt->month = 12;
        dt->day = _days_in_month(dt->year, dt->month);
    }

    /* set day of the week */
    dt->day_of_week = day_of_week(dt->year, dt->month, dt->day);

    calcalc_log("Local Time %d/%d/%d DOW:%d %d:%d:%d\r\n", dt->month, dt->day, dt->year, dt->day_of_week, dt->hour, dt->minute, dt->second);

    return CalendarCalcs::CalendarCalcs_Error::NONE;
}

/* determine if the month value is valid ------------------------------------- */
bool CalendarCalcs::_month_is_valid(int8_t month) {
    // month value cannot be less then January
    if (month < CalendarCalcs::MONTH::JANUARY) {
        calcalc_log("Month %d is not a valid month\r\n", month);
        calcalc_log("Valid months are 1 to 12\r\n");

        return false;
    }


    // month value cannot be greater then December
    if (month > CalendarCalcs::MONTH::DECEMBER) {
        calcalc_log("Month %d is not a valid month\r\n", month);
        calcalc_log("Valid months are 1 to 12\r\n");

        return false;
    }

    return true;
}

/* return number of days in a month correcting for leap years ---------------- */
int8_t CalendarCalcs::_days_in_month(uint16_t year, int8_t month) {
    switch (month) {
        case CalendarCalcs::MONTH::JANUARY:
            return 31;
            break;
        
        case CalendarCalcs::MONTH::FEBRUARY:
            if (is_leap_year(year)) {
                return 28 + 1;
            } else {
                return 28;
            }
            break;

        case CalendarCalcs::MONTH::MARCH:
            return 31;
            break;

        case CalendarCalcs::MONTH::APRIL:
            return 30;
            break;

        case CalendarCalcs::MONTH::MAY:
            return 31;
            break;

        case CalendarCalcs::MONTH::JUNE:
            return 30;
            break;

        case CalendarCalcs::MONTH::JULY:
            return 31;
            break;

        case CalendarCalcs::MONTH::AUGUST:
            return 31;
            break;

        case CalendarCalcs::MONTH::SEPTEMBER:
            return 30;
            break;

        case CalendarCalcs::MONTH::OCTOBER:
            return 31;
            break;

        case CalendarCalcs::MONTH::NOVEMBER:
            return 30;
            break;

        case CalendarCalcs::MONTH::DECEMBER:
            return 31;
            break;
                    
        default:
            return CALENDARCALCS_ERROR;
            break;
    }
}

/* determine if the day value is valid --------------------------------------- */
bool CalendarCalcs::_day_is_valid(uint16_t year, int8_t month, int8_t day) {
    // day cannot be less then or equal to zero
    if (day <= 0) {
        calcalc_log("Day is zero\r\n");

        return false;
    }

    // check for valid month first
    if (!_month_is_valid(month)) {
        return false;
    }

    int8_t max_days = _days_in_month(year, month);

    if (day > max_days) {
        calcalc_log("Day %d is not a valid day for month %d\r\n", day, month);
        calcalc_log("Valid days are 1 to %d\r\n", max_days);

        return false;
    }

    return true;
}

/* return standard time UTC offset for given timezone ------------------------ */
int8_t CalendarCalcs::_utc_offset_standard(CalendarCalcs::TIMEZONE tz) {
    switch (tz) {
        case CalendarCalcs::TIMEZONE::EST:
            return _est_utc_offset;
            break;
        
        case CalendarCalcs::TIMEZONE::MST:
            return _mst_utc_offset;
            break;

        case CalendarCalcs::TIMEZONE::MST_AZ:
            return _mst_az_utc_offset;
            break;

        case CalendarCalcs::TIMEZONE::CST:
            return _cst_utc_offset;
            break;

        case CalendarCalcs::TIMEZONE::PST:
            return _pst_utc_offset;
            break;

        case CalendarCalcs::TIMEZONE::AKST:
            return _akst_utc_offset;
            break;

        case CalendarCalcs::TIMEZONE::HST:
            return _hst_utc_offset;
            break;

        default:
            return CALENDARCALCS_ERROR;
            break;
    }
}

/* increment the day of the week --------------------------------------------- */
void CalendarCalcs::_increment_day_of_week(CalendarCalcs::DAY_OF_WEEK *dow) {
    switch (*dow) {  
        case CalendarCalcs::DAY_OF_WEEK::SUNDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::MONDAY;
            break;
        
        case CalendarCalcs::DAY_OF_WEEK::MONDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::TUESDAY;
            break;
    
        case CalendarCalcs::DAY_OF_WEEK::TUESDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::WEDNESDAY;
            break;

        case CalendarCalcs::DAY_OF_WEEK::WEDNESDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::THURSDAY;
            break;
    
        case CalendarCalcs::DAY_OF_WEEK::THURSDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::FRIDAY;
            break;
        
        case CalendarCalcs::DAY_OF_WEEK::FRIDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::SATURDAY;
            break;
        
        case CalendarCalcs::DAY_OF_WEEK::SATURDAY:
            *dow = CalendarCalcs::DAY_OF_WEEK::SUNDAY;
            break;

        default:
            *dow = CalendarCalcs::DAY_OF_WEEK::DOW_ERROR;
            break;
    }
}

/* return the date for the nth day of the week for a specific month ---------- */
int8_t CalendarCalcs::_ordinal_day_of_month(uint8_t nth, CalendarCalcs::DAY_OF_WEEK nth_dow, 
                                            int16_t year, int8_t month) {    
    // make sure month is valid
    if (!_month_is_valid(month)) {
        return CALENDARCALCS_ERROR;
    }

    // there will never be a 6th or greater day of the week
    if (nth >= 6) {
        return CALENDARCALCS_ERROR;
    }

    // get the first day of the month
    CalendarCalcs::DAY_OF_WEEK first_day_of_month = day_of_week(year, month, 1);
    uint8_t ordinal = 1;    // start with the 1st of any day
    CalendarCalcs::DAY_OF_WEEK current_day_of_week = first_day_of_month;

    // iterate through all the days of the month
    for (uint8_t i=1; i<=_days_in_month(year, month); i++) {
        // check if we have found the nth day of the week
        if (ordinal == nth) {
            if (current_day_of_week == nth_dow) {
                return i;
            }
        }

        _increment_day_of_week(&current_day_of_week);

        // increment ordinal if a week has passed
        if (current_day_of_week == first_day_of_month) {
            ordinal++;
        }
    }

    // cannot find that ordinal
    return CALENDARCALCS_ERROR;
}