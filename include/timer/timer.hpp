#pragma once
#include <stdint.h>
namespace PIT {
    void init(uint16_t frequency);
}

namespace Time {
    void millis();
    uint32_t seconds(); 
    uint32_t getTimer();
    void resetTimer();
}