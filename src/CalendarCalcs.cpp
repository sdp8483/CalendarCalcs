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
/* https://www.almanac.com/how-find-day-week */
CalendarCalcs::DAY_OF_WEEK CalendarCalcs::day_of_week(uint16_t year, uint8_t month, uint8_t day) {
    // check validity of month
    if (!_month_is_valid(month)) {
        return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_MONTH;
    }

    // check validity of day
    if (!_day_is_valid(year, month, day)) {
        return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_DAY;
    }

    // Does not work on dates before 1753
    if (year <= 1753) {
        return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR_YEAR;
    }

    // get the last two digits in year
    uint8_t year_end_digits = year % 100;

    // quarter of those two digits, discard remainder
    uint8_t quarter_year_digits = (uint8_t)(year_end_digits / 4);

    // add numbers together along with month key and day
    uint16_t sum = year_end_digits + quarter_year_digits + day + _month_key[month];

    // month keys are offset if it is a leap year for january and february
    if (is_leap_year(year) && (month <= CalendarCalcs::MONTH::FEBRUARY)) {
        sum += _month_key_leap_year_offset;
    }

    // if the year is between 2000 and 2099 then subtract 1
    sum -= 1;

    uint8_t dow = sum % 7;

    switch (dow) {
        case 1:
            return CalendarCalcs::DAY_OF_WEEK::SUNDAY;
            break;
    
        case 2:
            return CalendarCalcs::DAY_OF_WEEK::MONDAY;
            break;
        
        case 3:
            return CalendarCalcs::DAY_OF_WEEK::TUESDAY;
            break;
    
        case 4:
            return CalendarCalcs::DAY_OF_WEEK::WEDNESDAY;
            break;

        case 5:
            return CalendarCalcs::DAY_OF_WEEK::THURSDAY;
            break;
    
        case 6:
            return CalendarCalcs::DAY_OF_WEEK::FRIDAY;
            break;
        
        case 7:
            return CalendarCalcs::DAY_OF_WEEK::SATURDAY;
            break;

        default:
            return CalendarCalcs::DAY_OF_WEEK::DOW_ERROR;
            break;
    }
}

/* determine if it is daylight savings time ---------------------------------- */
/* https://www.nist.gov/pml/time-and-frequency-division/popular-links/daylight-savings-time-dst */
CalendarCalcs::DST CalendarCalcs::is_daylight_savings(CalendarCalcs::TIMEZONE tz, uint16_t year, uint8_t month, uint8_t day, uint8_t utc_hour) {
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
    // daylight savings time ends at 2am 1st Sunday in November
    int8_t first_sunday_in_november = _ordinal_day_of_month(1, CalendarCalcs::DAY_OF_WEEK::SUNDAY, year, CalendarCalcs::MONTH::NOVEMBER);

    // get the local hour in standard time
    uint8_t local_hour_stdTime = utc_hour + _utc_offset_standard(tz);

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

/* determine if the month value is valid ------------------------------------- */
bool CalendarCalcs::_month_is_valid(uint8_t month) {
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
int8_t CalendarCalcs::_days_in_month(uint8_t year, uint8_t month) {
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
bool CalendarCalcs::_day_is_valid(uint8_t year, uint8_t month, uint8_t day) {
    // day cannot be zero
    if (day == 0) {
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
                                       uint8_t year, uint8_t month) {
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
                return current_day_of_week;
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