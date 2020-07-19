
#pragma once
#include <Arduino.h>

namespace BusDashboard {
    class KomsiCommandListener {
    public:
        virtual void receiveCommand(const uint8_t command, const uint8_t value) = 0;
    };

    class KomsiCommandListenerNode {
    public:
        KomsiCommandListenerNode* next() { return _next; }
        KomsiCommandListenerNode* append(KomsiCommandListener& item) {
            _next = new KomsiCommandListenerNode(item);
            return _next;
        }
        KomsiCommandListener* item() { return _item; }
        KomsiCommandListenerNode(KomsiCommandListener& item):_item(&item), _next(nullptr) {};
    
    private:
        KomsiCommandListener* _item;
        KomsiCommandListenerNode* _next;

        KomsiCommandListenerNode() = delete;
        KomsiCommandListenerNode(const KomsiCommandListenerNode&) = delete;
        KomsiCommandListenerNode& operator=(const KomsiCommandListenerNode&) = delete;
    };
}