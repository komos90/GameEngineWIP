#include <iostream>
#include "prof_timer.h"
#include "time_manager.h"

ProfTimer::ProfTimer()
    : averageTime_(0)
    , n_(1)
{

}

void ProfTimer::start() {
    startTime_ = S64(gTimeManager.getTicks());
}

void ProfTimer::stop() {
    // Cumulative moving average
    S64 delta = gTimeManager.getTicks() - startTime_;
    //std::cout << "delta: " << std::to_string(delta) << std::endl;
    averageTime_ = averageTime_ + (delta - averageTime_) / n_;
    //std::cout << "averageTime: " << std::to_string(averageTime_ / gTimeManager.getTicksPerSecond()) << std::endl;
    ++n_;
}

std::string ProfTimer::toString() const {
    return "Average time (ms): " + std::to_string(1000.0 * (F64(averageTime_) / gTimeManager.getTicksPerSecond()));
}

void ProfTimer::print(const std::string& name) const {
    std::cout << "[" << name <<  "] " << toString() << std::endl;
}