#include "MoveRecommender.h"
#include <cmath>

MoveRecommender::MoveRecommender(Board &board, int maxDepth)
    : _board(board), _maxDepth(maxDepth) {}

int MoveRecommender::getPieceValue(char piece) const {
    switch (tolower(piece)) {
        case 'p': return 10;
        case 'n': return 30;
        case 'b': return 30;
        case 'r': return 50;
        case 'q': return 90;
        case 'k': return 900;
        default: return 0;
    }
}

bool MoveRecommender::isPieceThreatenedByWeaker(int x, int y, Board &board, bool isWhite) {
    int pieceValue = getPieceValue(board.getPieceSymbol(x, y));
    for (const auto& [pos, piece] : board.getPieces()) {
        if (piece->getIsWhite() == isWhite) continue;
        if (piece->isValidMove(pos.first, pos.second, x, y, board.toString())) {
            if (getPieceValue(piece->getSymbol()) < pieceValue) return true;
        }
    }
    return false;
}

bool MoveRecommender::isThreateningStrongerPiece(int x, int y, Board &board, bool isWhite) {
    for (const auto& [pos, piece] : board.getPieces()) {
        if (piece->getIsWhite() != isWhite) continue;
        if (piece->isValidMove(x, y, pos.first, pos.second, board.toString())) {
            if (getPieceValue(piece->getSymbol()) < getPieceValue(board.getPieceSymbol(pos.first, pos.second)))
                return true;
        }
    }
    return false;
}

int MoveRecommender::evaluateMove(int fromX, int fromY, int toX, int toY, int depth, bool isWhiteTurn) {
    if (!_board.getPiece(fromX, fromY) ||
        !_board.getPiece(fromX, fromY)->isValidMove(fromX, fromY, toX, toY, _board.toString()))
        return -1;

    char captured = _board.getPieceSymbol(toX, toY);
    Board simulated = _board.simulateMove(fromX, fromY, toX, toY);
    int score = 0;

    if (captured != '#') score += CAPTURE_SCORE;
    if (isThreateningStrongerPiece(toX, toY, simulated, isWhiteTurn)) score += THREATENS_STRONGER_PIECE;
    if (isPieceThreatenedByWeaker(toX, toY, simulated, isWhiteTurn)) score += UNDER_THREAT_BY_WEAKER;

    if (depth > 0)
        score -= getBestMoveScore(simulated, depth - 1, !isWhiteTurn);

    return score;
}

int MoveRecommender::getBestMoveScore(Board &board, int depth, bool isWhiteTurn) {
    int bestScore = -10000;
    for (const auto& [pos, piece] : board.getPieces()) {
        if (piece->getIsWhite() != isWhiteTurn) continue;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int score = evaluateMove(pos.first, pos.second, i, j, depth, isWhiteTurn);
                bestScore = std::max(bestScore, score);
            }
        }
    }
    return bestScore == -10000 ? 0 : bestScore;
}

void MoveRecommender::calculateMoves(bool isWhiteTurn, int numMoves) {
    for (const auto& [pos, piece] : _board.getPieces()) {
        if (piece->getIsWhite() != isWhiteTurn) continue;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int score = evaluateMove(pos.first, pos.second, i, j, _maxDepth, isWhiteTurn);
                if (score >= 0) {
                    std::string moveStr = toNotation(pos.first, pos.second) + toNotation(i, j);
                    _topMoves.push(Move(moveStr, score));
                }
            }
        }
    }
    // Keep only top N
    while (_topMoves.size() > numMoves) _topMoves.poll();
}

const PriorityQueue<Move, MoveComparator> &MoveRecommender::getTopMoves() const {
    return _topMoves;
}

std::ostream &operator<<(std::ostream &os, const MoveRecommender &recommender) {
    PriorityQueue<Move, MoveComparator> temp = recommender._topMoves;
    int rank = 1;
    while (!temp.empty()) {
        os << rank++ << ". " << temp.poll() << '\n';
    }
    return os;
}

std::string MoveRecommender::toNotation(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;
    return std::string{file, rank};
}
