#include "MoveRecommender.h"
#include <cmath>
#include "ThreadPool.h"
#include <mutex>

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
    Piece* piece = _board.getPiece(fromX, fromY);
    if (!piece || !piece->isValidMove(fromX, fromY, toX, toY, _board.toString()))
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
    stopFlag.store(false);  // Ensure flag is reset
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<void>> futures;

    for (const auto& [pos, piecePtr] : _board.getPieces()) {
        if (!piecePtr) continue;
        Piece* piece = piecePtr.get();
        if (!piece || piece->getIsWhite() != isWhiteTurn) continue;

        futures.emplace_back(pool.enqueue([this, position = pos, isWhiteTurn]() {
            for (int i = 0; i < 8 && !stopFlag.load(); ++i) {
                for (int j = 0; j < 8 && !stopFlag.load(); ++j) {
                    int score = evaluateMove(position.first, position.second, i, j, _maxDepth, isWhiteTurn);
                    if (score >= 0) {
                        std::string moveStr = toNotation(position.first, position.second) + toNotation(i, j);
                        {
                            std::lock_guard<std::mutex> lock(queueMutex);
                            _topMoves.push(Move(moveStr, score));
                        }
                        if (score >= THRESHOLD_SCORE) {
                            stopFlag.store(true);
                            return;
                        }
                    }
                }
            }
        }));
    }

    for (auto& f : futures) f.get();

    while (_topMoves.size() > numMoves) {
        _topMoves.poll();  // retain top N
    }
}

void MoveRecommender::calculateMovesSingleThreaded(bool isWhiteTurn, int numMoves) {
    stopFlag.store(false);
    for (const auto& [pos, piece] : _board.getPieces()) {
        if (!piece || piece->getIsWhite() != isWhiteTurn) continue;
        for (int i = 0; i < 8 && !stopFlag.load(); ++i) {
            for (int j = 0; j < 8 && !stopFlag.load(); ++j) {
                int score = evaluateMove(pos.first, pos.second, i, j, _maxDepth, isWhiteTurn);
                if (score >= 0) {
                    std::string moveStr = toNotation(pos.first, pos.second) + toNotation(i, j);
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        _topMoves.push(Move(moveStr, score));
                    }
                    if (score >= THRESHOLD_SCORE) {
                        stopFlag.store(true);
                        return;
                    }
                }
            }
        }
    }

    while (_topMoves.size() > numMoves) {
        _topMoves.poll();
    }
}

const PriorityQueue<Move, MoveComparator>& MoveRecommender::getTopMoves() const {
    return _topMoves;
}

std::ostream& operator<<(std::ostream& os, const MoveRecommender& recommender) {
    std::lock_guard<std::mutex> lock(recommender.queueMutex);
    auto snapshot = recommender._topMoves.snapshot();
    int rank = 1;
    for (const auto& move : snapshot) {
        os << rank++ << ". " << move << '\n';
    }
    return os;
}

std::string MoveRecommender::toNotation(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;
    return std::string{file, rank};
}
