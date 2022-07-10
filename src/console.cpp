#include <chrono>
#include "console.h"
#include "Node.h"
#include <ctime>

std::string console::now()
{
    // TODO: investigate why these lines crash on CI
    // const std::chrono::time_point<std::chrono::local_t, std::chrono::seconds> time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
    // return std::format("{:%F %T}", time);

    auto now = std::chrono::system_clock::now();
    std::time_t raw_now = std::chrono::system_clock::to_time_t(now);
    std::tm *timeinfo = std::localtime(&raw_now);
    std::string buffer(32, ' ');
    buffer.resize(std::strftime(&buffer[0], buffer.size(), "%d-%m-%Y %H:%M:%S", timeinfo));
    return buffer;
}

bool console::lastFormattedInputFunction = false;