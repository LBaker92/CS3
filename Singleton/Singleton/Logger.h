#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>

class Logger {
public:
    void report(const std::string& text);

    static Logger& get_instance();
private:
    std::ofstream log_file;

    Logger() { };
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;
};


#endif