// example opening file for appending
// Mikhail Nesterenko
// 10/15/2017

#include "Logger.h"

int main() {
    Logger& log = Logger::get_instance();
    log.report("This is test #1.");
    log.report("This is test #2.");
}