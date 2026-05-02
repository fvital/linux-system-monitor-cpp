#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(double seconds)
{
    int int_sec = round(seconds);
    int h = int_sec / 3600;
    int m = (int_sec % 3600) / 60;
    int s = int_sec % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << h << ":"
        << std::setw(2) << m << ":"
        << std::setw(2) << s;
    return oss.str();
}