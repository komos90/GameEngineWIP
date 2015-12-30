#include "clock.h"
#include "time_manager.h"

Clock::Clock(F32 startTimeInSeconds) :
    timeInCycles_(secondsToCycles(startTimeInSeconds)),
    timeScale_(1.0f),
    isPaused_(false)
{
    
}

Clock::~Clock() {

}
U64 Clock::secondsToCycles(F32 seconds) {
    return static_cast<U64>(seconds * gTimeManager.getTicksPerSecond());
}
F32 Clock::cyclesToSeconds(U64 ticks) {
    return static_cast<F32>(ticks) / gTimeManager.getTicksPerSecond();
}
void Clock::update(F32 frameDeltaInSeconds) {
    if (isPaused_) return;
    timeInCycles_ += secondsToCycles(frameDeltaInSeconds * timeScale_);
}
void Clock::setPaused(bool wantPaused) {
    isPaused_ = wantPaused;
}
void Clock::setTimeScale(F32 timeScale) {
    timeScale_ = timeScale;
}
void Clock::singleStep() {
    timeInCycles_ += secondsToCycles((1.0f / 60.0f) * timeScale_);
}
U64 Clock::getTimeInCycles() const {
    return timeInCycles_;
}
F32 Clock::calcDeltaInSeconds(const Clock& other) const {
    return cyclesToSeconds(timeInCycles_ - other.getTimeInCycles());
}
bool Clock::getPaused() const {
    return isPaused_;
}
F32 Clock::getTimeScale() const {
    return timeScale_;
}