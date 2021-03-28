#pragma once
#include "PowerSupply.h"

namespace BusDashboard
{
    class PowerStateListener
    {
    public:
        virtual void receiveState(const PowerSupply::PowerState value) = 0;
    };

}