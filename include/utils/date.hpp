#ifndef UTILS_DATE_HPP
#define UTILS_DATE_HPP

#include <ctime>
#include <string>

namespace utils {

inline const std::string current_date() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
    return buf;
}

}  // namespace utils

#endif
