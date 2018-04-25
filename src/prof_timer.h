#pragma once
#include <iostream>
#include "common_imports.h"

class ProfTimer {
    S64 averageTime_;
    S64 n_;
    S64 startTime_;

public:
    explicit ProfTimer();
    void start();
    void stop();
    void print(const std::string& name = "") const;
    std::string toString() const;
};