#pragma once

#include <string>
#include <map>
#include <vector>

#include "common_imports.h"

class TimeManager {
private:
public:
    explicit TimeManager();
    ~TimeManager();
    void init();
    void destroy();

    U64 getTicks();
    U32 getTicksPerSecond();
    F32 getTimeDiffInSeconds(U64 start, U64 end);
    //getHighResTimer()
    //getFrameDelta()
    //Keep running average of last couple of frame deltas
    //etc.
};

extern TimeManager gTimeManager;