#include <chrono>
#include <ctime>
#include "console.h"

std::string console::now()
{
    auto now = std::chrono::system_clock::now();
    std::time_t raw_now = std::chrono::system_clock::to_time_t(now);
    std::tm *timeinfo = std::localtime(&raw_now);
    std::string buffer(64, ' ');
    buffer.resize(std::strftime(&buffer[0], buffer.size(), "%d-%m-%Y %H:%M:%S", timeinfo));
    return buffer;
}