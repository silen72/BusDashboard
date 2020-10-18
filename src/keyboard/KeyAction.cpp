#include "keyboard/KeyAction.h"

namespace BusDashboard
{
    void KeyAction::run()
    {
        if (!done())
        {
            if ((_waitFor == nullptr) || (millis() > _waitDelay + _waitFor->doneTimeStamp()))
            {
                doAction();
                setDone();
            }
        }
        else if (_dependant != nullptr)
        {
            _dependant->run();
        }
    }

    void KeyAction::setDone()
    {
        _doneTimeStamp = millis();
    }

    inline void KeyAction::waitFor(KeyAction &other)
    {
        waitFor(other, 250);
    }

    bool KeyAction::removable() const
    {
        if (_dependant == nullptr)
            return done();
        return done() && _dependant->removable();
    }

    void KeyAction::waitFor(KeyAction &other, const unsigned long delay)
    {
        _waitFor = &other;
        _waitDelay = delay;
        other._dependant = this;
    }

    KeyAction::~KeyAction()
    {
        if (_dependant != nullptr)
        {
            delete _dependant;
            _dependant = nullptr;
        }
    }

} // namespace BusDashboard