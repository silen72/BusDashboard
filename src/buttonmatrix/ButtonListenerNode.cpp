#include "buttonmatrix/ButtonListener.h"

namespace BusDashboard {
    ButtonListenerNode* ButtonListenerNode::append(ButtonListener& buttonlistener) {
        _next = new ButtonListenerNode(buttonlistener);
        return _next;
    }
}