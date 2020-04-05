#include <string>
#include <math.h>
#include <iostream>
#include <sstream>

#include "format.h"

using std::string;
using std::to_string;
using std::ostringstream;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long s) {
    int seconds = (int)s;
    int hr, min, sec; 
    hr = seconds/3600;
    min = (seconds % 3600) / 60;
    sec = (seconds % 3600) % 60;

    ostringstream oss;
    oss << hr << ":";
    if (min < 10){
        oss << "0";
    }
    oss << min << ":";
    if (sec < 10){
        oss << "0";
    }
    oss << sec;

    return oss.str();
}