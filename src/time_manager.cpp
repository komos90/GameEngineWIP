#include <windows.h>

#include "time_manager.h"
#include "common_imports.h"

// Global instance
TimeManager gTimeManager;


TimeManager::TimeManager() {
    ASSERT(this == &gTimeManager);
}
TimeManager::~TimeManager() {

}
void TimeManager::init() {

}
void TimeManager::destroy() {

}
U64 TimeManager::getTicks() {
    LARGE_INTEGER ticks;
    BOOL result = QueryPerformanceCounter(&ticks);
    ASSERT(result);
    return static_cast<U64>(ticks.QuadPart);
}
U32 TimeManager::getTicksPerSecond() {
    LARGE_INTEGER freq;
    BOOL result = QueryPerformanceFrequency(&freq);
    ASSERT(result);
    return static_cast<U32>(freq.QuadPart);
}
F32 TimeManager::getTimeDiffInSeconds(U64 start, U64 end) {
    ASSERT(end >= start);
    F32 diff = static_cast<F32>(end - start);
    return diff / static_cast<F32>(getTicksPerSecond());
}
