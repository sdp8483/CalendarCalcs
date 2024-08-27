cmake_minimum_required(VERSION 3.28)

add_executable(unit_tests
               unit_tests.cpp
               calc_tests.cpp)

target_link_libraries(unit_tests PRIVATE CalendarCalcs)

# Leap Year Unit Tests
add_test(NAME LeapYearCheck2024             COMMAND unit_tests 1)
add_test(NAME LeapYearCheck2025             COMMAND unit_tests 2)
add_test(NAME LeapYearCheck2100             COMMAND unit_tests 3)
add_test(NAME LeapYearCheck2000             COMMAND unit_tests 4)

# Day of the Week Tests
add_test(NAME Thursday_Febuary_29_2024      COMMAND unit_tests 5)
add_test(NAME Monday_August_26_2024         COMMAND unit_tests 6)
add_test(NAME Wednesday_April_10_1878       COMMAND unit_tests 7)
add_test(NAME Sunday_July_22_2895           COMMAND unit_tests 8)
add_test(NAME DayOfWeek_Error_Month_13      COMMAND unit_tests 9)
add_test(NAME DayOfWeek_Error_Month_0       COMMAND unit_tests 10)
add_test(NAME DayOfWeek_Error_Sept_31       COMMAND unit_tests 11)
add_test(NAME DayOfWekk_Error_Feb_29_2023   COMMAND unit_tests 12)

# Daylight Savings Tests
add_test(NAME Not_DST_March_10_2024_1am     COMMAND unit_tests 13)
add_test(NAME Is_DST_March_10_2024_2am      COMMAND unit_tests 14)
add_test(NAME Is_DST_November_3_2024_1am    COMMAND unit_tests 15)
add_test(NAME Not_DST_November_2_2024_2am   COMMAND unit_tests 16)