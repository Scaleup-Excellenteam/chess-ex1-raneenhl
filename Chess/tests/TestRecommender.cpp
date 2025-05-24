#include "MoveRecommender.h"
#include "CustomExeptions.h"
#include <cassert>
#include <iostream>

void testEmptyQueueException() {
    PriorityQueue<Move, MoveComparator> pq;
    bool caught = false;
    try {
        pq.poll();
    } catch (const EmptyQueueException& e) {
        caught = true;
        std::cout << "Caught expected EmptyQueueException: " << e.what() << '\n';
    }
    assert(caught && "EmptyQueueException was not thrown on poll from empty queue");
}

void testMoveRecommenderSimple() {
    std::string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Board b(board, true);
    MoveRecommender r(b, 1);
    r.calculateMoves(true, 3);
    auto top = r.getTopMoves();
    assert(top.size() <= 3);
    std::cout << "Top moves generated (max 3):\n" << r << std::endl;
}

int main() {
    testEmptyQueueException();
    testMoveRecommenderSimple();
    std::cout << "All tests passed.\n";
    return 0;
}
