#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds)
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << h << ":"
        << std::setw(2) << m << ":"
        << std::setw(2) << s;
    return oss.str();
}