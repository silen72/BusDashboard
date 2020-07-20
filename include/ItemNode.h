#pragma once

template <class T>
class ItemNode
{
public:
    ItemNode<T> *append(T &item)
    {
        ItemNode<T> *newnode = nullptr;
        if (nullptr == _next) {
            _next = new ItemNode(item);
            newnode = _next;
        } else {
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