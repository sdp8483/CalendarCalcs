cmake_minimum_required(VERSION 3.28)

# library core sources
set(CALENDARCALC_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/CalendarCalcs.cpp)

# include directories
set(CALENDARCALC_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR})

# register library to the system
add_library(CalendarCalcs INTERFACE)
target_sources(CalendarCalcs INTERFACE ${CALENDARCALC_SRCS})
target_include_directories(CalendarCalcs INTERFACE ${CALENDARCALC_INCLUDES})