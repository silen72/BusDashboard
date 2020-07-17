/**
 *
 *
 */ 

#pragma once
#include <stdint.h>

namespace BusDashboard {
    class KomsiCommandListener {
    public:
        virtual void receiveCommand(const uint8_t command, const uint8_t value) = 0;
    };
}