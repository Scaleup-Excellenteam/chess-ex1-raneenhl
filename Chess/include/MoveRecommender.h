#pragma once

#include "Board.h"
#include "PriorityQueue.h"
#include "Move.h"
#include <iostream>
#include <atomic>
#include "ThreadPool.h"

class MoveRecommender;

// Allow easy printing of top recommended moves
std::ostream &operator<<(std::ostream &os, const MoveRecommender &recommender);

/**
 * Recommends the best moves based on heuristic evaluation.
 * Supports both single-threaded and multithreaded evaluation using a ThreadPool.
 */
class MoveRecommender {
private:
    Board &_board;                         // Reference to the current board state
    int _maxDepth;                         // Max recursion depth for evaluation
    PriorityQueue<Move, MoveComparator> _topMoves; // Priority queue of top recommended moves
    mutable std::mutex queueMutex;         // Synchronization for multithreaded insertion

    std::atomic<bool> stopFlag = false;    // Stop signal (not currently used)
    static constexpr int THRESHOLD_SCORE = 500; // Unused constant — could define early pruning limit

    static const int CAPTURE_SCORE = 100;               // Capturing any piece
    static const int THREATENS_STRONGER_PIECE = 50;     // Threatening more valuable piece
    static const int UNDER_THREAT_BY_WEAKER = -70;      // Exposing valuable piece to weak attacker

    /**
     * Returns material value of a piece for comparison.
     */
    int getPieceValue(char piece) const;

    /**
     * Evaluates a single move using heuristics and recursion.
     * If depth > 0, applies minimax via getBestMoveScore.
     */
    int evaluateMove(int fromX, int fromY, int toX, int toY, int depth, bool isWhiteTurn);

    /**
     * Checks if a piece is currently under threat by a weaker opposing piece.
     */
    bool isPieceThreatenedByWeaker(int x, int y, Board &board, bool isWhite);

    /**
     * Checks if the moved piece threatens a stronger opponent piece.
     */
    bool isThreateningStrongerPiece(int x, int y, Board &board, bool isWhite);

    /**
     * Recursively evaluates the best move score from a given board state.
     */
    int getBestMoveScore(Board &board, int depth, bool isWhiteTurn);

public:
    /**
     * Constructor for a move recommender for a specific board and depth.
     */
    MoveRecommender(Board &board, int maxDepth = 2);

    /**
     * Generates and scores all possible legal moves for the current player.
     * Uses multithreading if a ThreadPool is provided.
     */
    void calculateMoves(bool isWhiteTurn, int numMoves, ThreadPool* pool = nullptr);

    /**
     * Returns reference to internal priority queue (read-only).
     */
    const PriorityQueue<Move, MoveComparator> &getTopMoves() const;

    /**
     * Returns a **copy** of the priority queue, useful for iteration outside the class.
     */
    PriorityQueue<Move, MoveComparator> getTopMovesCopy() const {
        return _topMoves;
    }

    /**
     * Converts board position to chess notation (e.g., row=7, col=0 → "a1").
     */
    std::string toNotation(int row, int col);

    /**
     * Fallback for environments without threads. Uses single-threaded brute-force evaluation.
     */
    void calculateMovesSingleThreaded(bool isWhiteTurn, int numMoves);

    // For printing the recommender (top N moves)
    friend std::ostream &operator<<(std::ostream &os, const MoveRecommender &recommender);
};
