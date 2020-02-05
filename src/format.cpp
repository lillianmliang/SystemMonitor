#include <string>
#include <sstream>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long hh, mm, ss;
    hh = seconds/3600;
    mm = seconds/60 - (hh*60);
    ss = seconds % 60;
    std::ostringstream stream;
    stream.width(2);
    stream.fill('0');
    stream.flags(std::ios::right);
    stream<<hh<<":";
    stream.width(2);
    stream.fill('0');
    stream.flags(std::ios::right);
    stream<<mm<<":";
    stream.width(2);
    stream.fill('0');
    stream.flags(std::ios::right);
    stream<<ss;
    return stream.str(); 
}