#include "Logger.h"

#include <iostream>
#include <fstream>

Logger& Logger::get_instance() {
    static Logger log;
    return log;
}

void Logger::report(const std::string& text) {
    log_file.open("log.txt", std::fstream::out | std::fstream::app);
    if (!log_file) {
        std::cout << "An error occured when opening the file.\n";
        exit(1);
    }

    log_file << text << std::endl;
    log_file.flush();
    log_file.close();
}