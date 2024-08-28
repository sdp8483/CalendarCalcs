cmake_minimum_required(VERSION 3.28)

#setup project
project(CalendarCalcsPROJECT CXX)

if(NOT PROJECT_IS_TOP_LEVEL)
    add_subdirectory(src)
else()
    # unit testing
    include(CTest)

    # add src directory and rename to bin on build
    add_subdirectory(src bin)

    # add test directory
    add_subdirectory(test)
endif()