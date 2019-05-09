#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

#include <fstream>
#include <mutex>
#include <string>
#include "utils/utils.hpp"

class Log {
   public:
    Log(const std::string &filename) {
        file_.open(filename, std::ofstream::out | std::ofstream::app);
        file_ << "**** BEGIN LOGGING ON " << utils::current_date() << " "
              << utils::current_time() << std::endl;
    }

    ~Log() {
        file_ << "**** END LOGGING ON " << utils::current_date() << " "
              << utils::current_time() << std::endl
              << std::endl;
    }

    static Log *get() {
        static Log log("log.txt");
        return &log;
    }

    template <typename... Args>
    void debug(Args &&... args) {
        mtx_.lock();
        file_ << "[" << utils::current_time() << "]  [DEBUG]  ";
        ((file_ << args), ...) << std::endl;
        mtx_.unlock();
    }

    template <typename... Args>
    void info(Args &&... args) {
        mtx_.lock();
        file_ << "[" << utils::current_time() << "]  [INFO]   ";
        ((file_ << args), ...) << std::endl;
        mtx_.unlock();
    }

    template <typename... Args>
    void warning(Args &&... args) {
        mtx_.lock();
        file_ << "[" << utils::current_time() << "]  [WARN]   ";
        ((file_ << args), ...) << std::endl;
        mtx_.unlock();
    }

    template <typename... Args>
    void error(Args &&... args) {
        mtx_.lock();
        file_ << "[" << utils::current_time() << "]  [ERROR]  ";
        ((file_ << args), ...) << std::endl;
        mtx_.unlock();
    }

   private:
    std::ofstream file_;
    std::mutex mtx_;
};

#endif
