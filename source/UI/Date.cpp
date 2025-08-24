#include "Date.h"
#include "File.h"
#include <ctime>

namespace MyBase  {
    Date::Date() { 
        std::time_t now = std::time(0);
        std::tm* local_time = std::localtime(&now);
        __year = local_time->tm_year+1900;
        __month = local_time->tm_mon+1;
        __day = local_time->tm_mday;
    }
    Date::Date(const unsigned int& year, const unsigned char& month, const unsigned char& day): __year(year), __month(month), __day(day) {}
    Date::~Date() {}

    Date Date::today() {
        Date date;
        return date;
    }
    const unsigned char& Date::getDay() const {
        return __day;
    }
    const unsigned char& Date::getMonth() const {
        return __month;
    }
    const unsigned int& Date::getYear() const {
        return __year;
    }
    void Date::input(File& file) {
        file >> __year;
        file >> __month;
        file >> __day;
    }
    void Date::output(File& file) const {
        file << __year;
        file << __month;
        file << __day;
    }
}