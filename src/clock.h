#pragma once

#include "common_imports.h"

class Clock {
private:
    U64 timeInCycles_;
    F32 timeScale_;
    bool isPaused_;
    // NOTE: Shoudl be in time manager class?
    static U64 secondsToCycles(F32 timeInSeconds);
    static F32 cyclesToSeconds(U64 timeInCycles);
public:
    explicit Clock(F32 startTimeInSeconds = 0.0f);
    ~Clock();

    void update(F32 frameDeltaInSeconds);
    void setPaused(bool wantPaused);
    void setTimeScale(F32 timeScale);
    void singleStep();
    U64 getTimeInCycles() const;
    F32 calcDeltaInSeconds(const Clock& other) const;
    bool getPaused() const;
    F32 getTimeScale() const;
};