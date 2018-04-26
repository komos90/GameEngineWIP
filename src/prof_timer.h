#pragma once
#include <iostream>
#include "common_imports.h"

class ProfTimer {
    S64 averageTime_;
    S64 n_;
    S64 startTime_;
    S32 printRate_;
    std::string name_;

public:
    explicit ProfTimer(const std::string& name = "", S32 printRate = -1);
    void start();
    void stop();
    void print() const;
    std::string toString() const;
};