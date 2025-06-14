#pragma once
#include <list>
#include <stdexcept>
#include <mutex>
#include <vector>
#include "CustomExeptions.h"

// Generic priority queue using a list and custom comparator
// Comparator should return positive if a > b, 0 if equal, negative if a < b
template <typename T, typename Comparator>
class PriorityQueue {
private:
    Comparator cmp;
    std::list<T> _elements;
    mutable std::mutex _mutex;

public:
    PriorityQueue() = default;

    // Copy constructor
    PriorityQueue(const PriorityQueue& other) {
        std::lock_guard<std::mutex> lock(other._mutex);
        _elements = other._elements;
    }

    // Copy assignment
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            std::lock_guard<std::mutex> lock1(_mutex);
            std::lock_guard<std::mutex> lock2(other._mutex);
            _elements = other._elements;
        }
        return *this;
    }

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

    std::vector<T> snapshot() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return std::vector<T>(_elements.begin(), _elements.end());
    }
};
