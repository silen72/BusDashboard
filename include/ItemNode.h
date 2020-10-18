#pragma once

template <class T>
class ItemNode
{
public:
    ItemNode<T> *append(T &item)
    {
        ItemNode<T> *newnode = nullptr;
        if (nullptr == _next)
        {
            _next = new ItemNode(item);
#ifdef SerialDebug
            Serial.print(F("ItemNode<T> *append() created new ItemNode "));
            Serial.println((int)_next);
#endif
            newnode = _next;
        }
        else
        {
#ifdef SerialDebug
            Serial.print(F("ItemNode<T> *append() passes item to "));
            Serial.println((int)_next);
#endif
            newnode = _next->append(item);
        }
        return newnode;
    }
    ItemNode<T> *next() { return _next; }
    T *item() { return _item; }
    ItemNode(T &item) : _next(nullptr), _item(&item) {}

private:
    ItemNode<T> *_next;
    T *_item;

    ItemNode() = delete;
    ItemNode(const ItemNode &) = delete;
    ItemNode &operator=(const ItemNode &) = delete;
};