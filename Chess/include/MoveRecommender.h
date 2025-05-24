#pragma once

#include "Board.h"
#include "PriorityQueue.h"
#include "Move.h"
#include <iostream>

class MoveRecommender;

std::ostream &operator<<(std::ostream &os, const MoveRecommender &recommender);

class MoveRecommender {
private:
    Board &_board;
    int _maxDepth;
    PriorityQueue<Move, MoveComparator> _topMoves;

    // Scoring for the moves
    static const int CAPTURE_SCORE = 100;
    static const int THREATENS_STRONGER_PIECE = 50;
    static const int UNDER_THREAT_BY_WEAKER = -70;

    int getPieceValue(char piece) const;
    int evaluateMove(int fromX, int fromY, int toX, int toY, int depth, bool isWhiteTurn);

    bool isPieceThreatenedByWeaker(int x, int y, Board &board, bool isWhite);
    bool isThreateningStrongerPiece(int x, int y, Board &board, bool isWhite);

    int getBestMoveScore(Board &board, int depth, bool isWhiteTurn);

public:
    MoveRecommender(Board &board, int maxDepth = 2);

    void calculateMoves(bool isWhiteTurn, int numMoves = 5);

    const PriorityQueue<Move, MoveComparator> &getTopMoves() const;

    friend std::ostream &operator<<(std::ostream &os, const MoveRecommender &recommender);

    std::string toNotation(int row, int col);
};