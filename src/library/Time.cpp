#include "../library.h"
#include <chrono>

void Time::init(stack *st)
{
    object::objectPtr Time = insertObject("Time"_n, nullptr);
    object::objectPtr now = (*Time)["sinceEpoch"_n] = object::makeEmptyObject();

    addFunctionL(now, "asFloatingPoint"_n, {
        double timeSinceEpoch = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::system_clock::now().time_since_epoch()).count();
        return object::makeObject(static_cast<number>(timeSinceEpoch));
    });

    addFunctionL(now, "asInteger"_n, {
        int timeSinceEpoch = std::chrono::duration_cast<std::chrono::duration<int>>(std::chrono::system_clock::now().time_since_epoch()).count();
        return object::makeObject(static_cast<number>(timeSinceEpoch));
    });
}