#ifndef UTILS_RAND_HPP
#define UTILS_RAND_HPP

namespace utils {

template <class T>
T rand_between(T a, T b) {
    return a + (b - a) * ((float)rand() / RAND_MAX);
}

}  // namespace utils

#endif
