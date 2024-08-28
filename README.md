# Calendar Calculations
Calculate common calendar and time based values for embedded systems. 

This library arose from working with the Raspberry Pi RP2040 that has an RTC but no way to correct for daylight savings time. Another needed feature was to turn a UTC timestamp from a GPS into local time.  I was unable to find a library to perform these calendar and time calculations, its also possible I did not use the right keywords to search for one. 

This library is only focused on US time zones and is only tested and verified for my local eastern time zone. This could change if someone finds this library useful.

 # Features
* Determine if a year is a leap year
* Calculate day of the week for a given date
* Determine if a given date and UTC time is during daylight savings time in the US
* **Not** optimized for size or speed

# Adding Library Using CMake
I prefer adding to my projects as a git submodule. Run the below command in the project root directory. Replace `SUBDIR/MORE_SUBDIR` with a subdirectory structure of your choice or place into root by omitting this part from the command.
```
git submodule add https://github.com/sdp8483/CalendarCalcs.git SUBDIR/MORE_SUBDIR
```

Edit the root `CMakeLists.txt` or the `CMakeLists.txt` in the `lib` directory with `add_subdirectory(CalendarCalcs)`

# Unit Testing
To unit test first run `cmake` in a build directory. Then the library must be built before `ctest` can be run:
```
mkdir build
cd build
cmake ..
make
ctest
```