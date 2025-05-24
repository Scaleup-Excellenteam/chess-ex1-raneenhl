#pragma once
#include <string>
#include <iostream>

struct Move {
    std::string notation;  // e.g., "e2e4"
    int score;

    Move(const std::string& n, int s) : notation(n), score(s) {}

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << move.notation << " (score: " << move.score << ")";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Move& move) {
        is >> move.notation;
        return is;
    }
};

// Comparator that ranks moves by score (higher is better)
struct MoveComparator {
    int operator()(const Move& a, const Move& b) const {
        return a.score - b.score;
    }
};
