#ifndef UTILS_TIME_HPP
#define UTILS_TIME_HPP

#include <ctime>
#include <string>

namespace utils {

inline const std::string current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

}  // namespace utils

#endif
