#pragma once
#include <exception>
#include <string>

class EmptyQueueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "PriorityQueue is empty. Cannot poll or access top element.";
    }
};

class InvalidMoveException : public std::exception {
private:
    std::string _msg;
public:
    explicit InvalidMoveException(const std::string& move)
        : _msg("Invalid move attempted: " + move) {}

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};
