#pragma once
#include <list>
#include <stdexcept>
#include "CustomExeptions.h"

// Generic priority queue with comparator
// T: item type, Comparator: function object returning int (a - b style)
template <typename T, typename Comparator>
class PriorityQueue {
private:
    Comparator cmp;
    std::list<T> _elements;


public:
    PriorityQueue() = default;

    void push(const T& value) {
        auto it = _elements.begin();
        for (; it != _elements.end(); ++it) {
            if (cmp(value, *it) > 0)  // value has higher priority than *it
                break;
        }
        _elements.insert(it, value);  // Insert before first lower-priority element
    }

    T poll() {
        if (_elements.empty()) {
            throw EmptyQueueException();
        }
        T top = _elements.front();
        _elements.pop_front();
        return top;
    }

    bool empty() const {
        return _elements.empty();
    }

    size_t size() const {
        return _elements.size();
    }

    const T& top() const {
        if (_elements.empty()) {
            throw EmptyQueueException();
        }
        return _elements.front();
    }

    typename std::list<T>::const_iterator begin() const {
        return _elements.begin();
    }
    typename std::list<T>::const_iterator end() const {
        return _elements.end();
    }

};
