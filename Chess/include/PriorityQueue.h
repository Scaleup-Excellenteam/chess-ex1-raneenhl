#pragma once
#include <list>
#include <stdexcept>
#include <mutex>
#include <vector>
#include "CustomExeptions.h"

// Generic priority queue with comparator
// T: item type, Comparator: function object returning int (a - b style)
template <typename T, typename Comparator>
class PriorityQueue {
private:
    Comparator cmp;
    std::list<T> _elements;
    mutable std::mutex _mutex;

public:
    PriorityQueue() = default;

    void push(const T& value) {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _elements.begin();
        for (; it != _elements.end(); ++it) {
            if (cmp(value, *it) > 0) break;
        }
        _elements.insert(it, value);
    }

    T poll() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_elements.empty()) throw EmptyQueueException();
        T top = _elements.front();
        _elements.pop_front();
        return top;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _elements.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _elements.size();
    }

    const T& top() const {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_elements.empty()) throw EmptyQueueException();
        return _elements.front();
    }

    typename std::list<T>::const_iterator begin() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _elements.begin();
    }

    typename std::list<T>::const_iterator end() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _elements.end();
    }

    std::vector<T> snapshot() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return std::vector<T>(_elements.begin(), _elements.end());
    }
};