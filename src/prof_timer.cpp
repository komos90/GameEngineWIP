#include <iostream>
#include "prof_timer.h"
#include "time_manager.h"

ProfTimer::ProfTimer(const std::string& name, S32 printRate)
    : averageTime_{0}
    , n_{1}
    , printRate_{printRate}
    , name_{name}
{

}

void ProfTimer::start() {
    startTime_ = S64(gTimeManager.getTicks());
}

void ProfTimer::stop() {
    // Cumulative moving average
    S64 delta = gTimeManager.getTicks() - startTime_;
    averageTime_ = averageTime_ + (delta - averageTime_) / n_;
    ++n_;
    if (printRate_ != -1 && n_ % printRate_ == 0) {
        print();
    }
}

std::string ProfTimer::toString() const {
    return "[" + name_ + "] " + "Average time (ms): " + std::to_string(1000.0 * (F64(averageTime_) / gTimeManager.getTicksPerSecond()));
}

void ProfTimer::print() const {
    std::cout <<  toString() << std::endl;
}