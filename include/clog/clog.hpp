#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include "../utils/date.hpp"
#include "../utils/time.hpp"

namespace clog {

class Log {
   public:
    static Log *get() {
        static Log log("log.txt");
        return &log;
    }

    template <typename... Args>
    void debug(Args &&... args) {
        write("debug", args...);
    }

    template <typename... Args>
    void info(Args &&... args) {
        write("info", args...);
    }

    template <typename... Args>
    void warn(Args &&... args) {
        write("warn", args...);
    }

    template <typename... Args>
    void error(Args &&... args) {
        write("error", args...);
    }

    template <typename... Args>
    void fatal(Args &&... args) {
        write("fatal", args...);
    }

   private:
    Log(const std::string filename) {
        std::unique_lock<std::mutex> lock(mtx_);
        file_.open(filename, std::ofstream::out | std::ofstream::app);
        file_ << "**** BEGIN LOGGING ON " << utils::current_date() << " " << utils::current_time()
              << " ****" << std::endl;
    }

    ~Log() {
        std::unique_lock<std::mutex> lock(mtx_);
        file_ << "**** END LOGGING ON " << utils::current_date() << " " << utils::current_time()
              << " ****" << std::endl
              << std::endl;
    }

    template <typename... Args>
    void write(const std::string &level, Args &&... args) {
        std::unique_lock<std::mutex> lock(mtx_);
        file_ << "[" << utils::current_time() << "]  [" << level << "]  ";
        ((file_ << args), ...) << std::endl;
    }

    std::ofstream file_;
    std::mutex mtx_;
};

}  // namespace clog

#endif
