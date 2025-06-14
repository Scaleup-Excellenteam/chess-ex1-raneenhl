#include "../include/Board.h"
#include "../include/Bishop.h"
#include "../include/Rook.h"
#include "../include/King.h"
#include "../include/Knight.h"
#include "../include/Pawn.h"
#include "../include/Queen.h"
#include <iostream>

Board::Board(const string& boardString, bool isWhiteTurn)
    : m_turnWhite(isWhiteTurn) {
    parseBoardString(boardString);
}

Board::Board(const Board& other) : m_turnWhite(other.m_turnWhite) {
    for (const auto& [pos, piece] : other.m_board) {
        m_board[pos] = std::unique_ptr<Piece>(piece->clone());
    }
}

Board::~Board() = default;

// Converts a 64-character board string into pieces on the internal board.
// Format: top-left to bottom-right, using single-char symbols ('#' for empty).
void Board::parseBoardString(const string& boardString) {
    m_board.clear();
    for (int i = 0; i < 64; ++i) {
        char c = boardString[i];
        if (c == '#') continue;

        bool isWhite = isupper(c);
        int row = 7 - (i / 8);
        int col = i % 8;

        switch (tolower(c)) {
            case 'b':
                m_board[{row, col}] = make_unique<Bishop>(isWhite);
                break;
            case 'k':
                m_board[{row, col}] =  make_unique<King>(isWhite);
                break;
            case 'n':
                m_board[{row, col}] = make_unique<Knight>(isWhite);
                break;
            case 'p':
                m_board[{row, col}] = make_unique<Pawn>(isWhite);
                break;
            case 'q':
                m_board[{row, col}] = make_unique<Queen>(isWhite);
                break;
            case 'r':
                m_board[{row, col}] = make_unique<Rook>(isWhite);
                break;            
        }
    }
}

 pair<int, int> Board::parsePosition(char file, char rank) {
    return { 8 - (rank - '0'), file - 'a' };
}

// Core move validation logic, including castling, promotion, and check detection.
// Applies the move directly to the current board if valid.
int Board::validateMove(const string& input) {
    if (input.length() != 4)
        return 21;  // Invalid input

    auto [fromRow, fromCol] = parsePosition(input[0], input[1]);
    auto [toRow, toCol] = parsePosition(input[2], input[3]);

    auto fromIt = m_board.find({fromRow, fromCol});
    if (fromIt == m_board.end())
        return 11;  // No piece at source

    Piece* piece = fromIt->second.get();
    if (piece->getIsWhite() != m_turnWhite)
        return 12;  // Not your piece

    auto toIt = m_board.find({toRow, toCol});
    if (toIt != m_board.end() && toIt->second->getIsWhite() == m_turnWhite)
        return 13;  // Destination occupied by your own piece

    // --- CASTLING CHECK ---
    if (tolower(piece->getSymbol()) == 'k' && !piece->getHasMoved()) {
        int row = fromRow;
        bool isWhite = piece->getIsWhite();

        // Short castling
        if (fromCol == 4 && toCol == 6) {
            auto rookIt = m_board.find({row, 7});
            if (rookIt != m_board.end()) {
                Piece* rook = rookIt->second.get();
                if (tolower(rook->getSymbol()) == 'r' && !rook->getHasMoved()) {
                    // Check if path is clear
                    if (!m_board.count({row, 5}) && !m_board.count({row, 6})) {
                        // Check if king passes through or lands in check
                        Board copy1 = simulateMove(fromRow, fromCol, row, 5);
                        Board copy2 = simulateMove(fromRow, fromCol, toRow, toCol);
                        if (!isKingInCheck(m_turnWhite) &&
                            !copy1.isKingInCheck(m_turnWhite) &&
                            !copy2.isKingInCheck(m_turnWhite)) {
                            // Execute castling
                            m_board[{toRow, toCol}] = std::move(m_board[{fromRow, fromCol}]);
                            m_board.erase({fromRow, fromCol});
                            m_board[{row, 5}] = std::move(m_board[{row, 7}]);
                            m_board.erase({row, 7});
                            m_board[{toRow, toCol}]->setHasMoved(true);
                            m_board[{row, 5}]->setHasMoved(true);
                            return 42;
                        }
                    }
                }
            }
        }

        // Long castling
        if (fromCol == 4 && toCol == 2) {
            auto rookIt = m_board.find({row, 0});
            if (rookIt != m_board.end()) {
                Piece* rook = rookIt->second.get();
                if (tolower(rook->getSymbol()) == 'r' && !rook->getHasMoved()) {
                    if (!m_board.count({row, 1}) && !m_board.count({row, 2}) && !m_board.count({row, 3})) {
                        Board copy1 = simulateMove(fromRow, fromCol, row, 3);
                        Board copy2 = simulateMove(fromRow, fromCol, toRow, toCol);
                        if (!isKingInCheck(m_turnWhite) &&
                            !copy1.isKingInCheck(m_turnWhite) &&
                            !copy2.isKingInCheck(m_turnWhite)) {
                            m_board[{toRow, toCol}] = std::move(m_board[{fromRow, fromCol}]);
                            m_board.erase({fromRow, fromCol});
                            m_board[{row, 3}] = std::move(m_board[{row, 0}]);
                            m_board.erase({row, 0});
                            m_board[{toRow, toCol}]->setHasMoved(true);
                            m_board[{row, 3}]->setHasMoved(true);
                            return 42;
                        }
                    }
                }
            }
        }
    }

    // --- Normal move ---
    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, toString()))
        return 21;

    auto movedPiece = move(m_board[{fromRow, fromCol}]);
    auto capturedPiece = m_board.count({toRow, toCol}) ? move(m_board[{toRow, toCol}]) : nullptr;

    m_board[{toRow, toCol}] = move(movedPiece);
    m_board[{toRow, toCol}]->setHasMoved(true);

    // Promotion
    Piece* promoted = m_board[{toRow, toCol}].get();
    if (tolower(promoted->getSymbol()) == 'p') {
        if ((promoted->getIsWhite() && toRow == 0) || (!promoted->getIsWhite() && toRow == 7)) {
            bool isWhite = promoted->getIsWhite();
            m_board[{toRow, toCol}] = std::make_unique<Queen>(isWhite);
            std::cout << (isWhite ? "White" : "Black") << " pawn promoted to queen!\n";
        }
    }

    m_board.erase({fromRow, fromCol});

    if (isKingInCheck(m_turnWhite)) {
        m_board[{fromRow, fromCol}] = move(m_board[{toRow, toCol}]);
        if (capturedPiece)
            m_board[{toRow, toCol}] = move(capturedPiece);
        else
            m_board.erase({toRow, toCol});
        return 31;
    }

    if (isKingInCheck(!m_turnWhite)) {
        if (!hasAnyLegalMove(!m_turnWhite)) {
            cout << "Checkmate! Game Over.\n";
            return 41;
        }
        return 41;
    }

    return 42;
}

bool Board::isKingInCheck(bool whiteKing) const {
    // Find the king
    int kingRow = -1, kingCol = -1;
    for (const auto& [pos, piece] : m_board) {
        if (piece->getIsWhite() == whiteKing && tolower(piece->getSymbol()) == 'k') {
            kingRow = pos.first;
            kingCol = pos.second;
            break;
        }
    }

    if (kingRow == -1 || kingCol == -1) return true; // king not found

    // Check all opponent pieces — can they attack the king?
    for (const auto& [pos, piece] : m_board) {
        if (piece->getIsWhite() != whiteKing) {
            if (piece->isValidMove(pos.first, pos.second, kingRow, kingCol, toString()))
                return true; // king is under attack
        }
    }

    return false;
}

string Board::toString() const {
     string result(64, '#'); // Start with an empty board

    for (const auto& [pos, piece] : m_board) {
        if (!piece) continue;
        int row = pos.first;
        int col = pos.second;
        if (piece) result[row * 8 + col] = piece->getSymbol();
    }

    return result;
}

bool Board::hasAnyLegalMove(bool forWhite) {
    for (const auto& [fromPos, piece] : m_board) {
        if (piece->getIsWhite() != forWhite)
            continue;

        int fromRow = fromPos.first;
        int fromCol = fromPos.second;

        for (int toRow = 0; toRow < 8; ++toRow) {
            for (int toCol = 0; toCol < 8; ++toCol) {
                if (fromRow == toRow && fromCol == toCol)
                    continue;

                // Check if destination has own piece
                auto it = m_board.find({toRow, toCol});
                if (it != m_board.end() && it->second->getIsWhite() == forWhite)
                    continue;

                if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, toString()))
                    continue;

                // Simulate move
                auto movedPiece =  move(m_board.at({fromRow, fromCol}));
                auto capturedPiece = m_board.count({toRow, toCol}) ?  move(m_board.at({toRow, toCol})) : nullptr;

                m_board[{toRow, toCol}] =  move(movedPiece);
                m_board.erase({fromRow, fromCol});

                bool stillInCheck = isKingInCheck(forWhite);

                // Undo
                m_board[{fromRow, fromCol}] =  move(m_board[{toRow, toCol}]);
                if (capturedPiece)
                    m_board[{toRow, toCol}] =  move(capturedPiece);
                else
                    m_board.erase({toRow, toCol});

                if (!stillInCheck)
                    return true;  // at least one legal move exists
            }
        }
    }

    return false;  // no legal move avoids check → checkmate
}

char Board::getPieceSymbol(int row, int col) const {
    auto it = m_board.find({row, col});
    if (it != m_board.end() && it->second) {
        return it->second->getSymbol();
    }
    return '#';  // empty square
}

Piece* Board::getPiece(int row, int col) const {
    auto it = m_board.find({row, col});
    return (it != m_board.end()) ? it->second.get() : nullptr;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    m_board[{toRow, toCol}] = std::move(m_board[{fromRow, fromCol}]);
    m_board.erase({fromRow, fromCol});
    Piece* moved = getPiece(toRow, toCol);
    if (moved) moved->setHasMoved(true);
}

Board Board::simulateMove(int fromRow, int fromCol, int toRow, int toCol) const {
    Board newBoard(this->toString(), m_turnWhite);

    // Get the piece from the original board
    Piece* originalPiece = getPiece(fromRow, fromCol);
    if (!originalPiece) return newBoard;  // Defensive: if no piece, just return unchanged board

    // Clone the piece
    std::unique_ptr<Piece> clonedPiece = originalPiece->clone();

    if (tolower(clonedPiece->getSymbol()) == 'p') {
        if ((clonedPiece->getIsWhite() && toRow == 0) || (!clonedPiece->getIsWhite() && toRow == 7)) {
            newBoard.m_board[{toRow, toCol}] = std::make_unique<Queen>(clonedPiece->getIsWhite());
        }
    }

    // Move the cloned piece into the new board
    newBoard.m_board[std::make_pair(toRow, toCol)] = std::move(clonedPiece);
    newBoard.m_board.erase(std::make_pair(fromRow, fromCol));

    Piece* moved = getPiece(toRow, toCol);
    if (moved) moved->setHasMoved(true);

    return newBoard;
}

const std::map<std::pair<int, int>, std::unique_ptr<Piece>>& Board::getPieces() const {
    return m_board;
}

bool Board::isLegalMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) const {
    if (!isInBounds(fromRow, fromCol) || !isInBounds(toRow, toCol))
        return false;

    auto piece = getPiece(fromRow, fromCol);
    if (!piece || piece->getIsWhite() != isWhiteTurn)
        return false;

    // Destination occupied by same color
    auto destPiece = getPiece(toRow, toCol);
    if (destPiece && destPiece->getIsWhite() == isWhiteTurn)
        return false;

    if (!piece->isLegalMove(fromRow, fromCol, toRow, toCol, *this))
        return false;

    // Simulate the move and check if king is in check
    Board simulated = simulateMove(fromRow, fromCol, toRow, toCol);
    if (simulated.isKingInCheck(isWhiteTurn))
        return false;

    return true;
}

bool Board::isInBounds(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

void Board::setTurn(bool isWhite) {
    this->m_turnWhite = isWhite;  
}

bool Board::getTurn() const { 
    return m_turnWhite; 
}

bool Board::isInCheck(bool isWhite) const {
    return isKingInCheck(isWhite);
}

bool Board::isInCheckmate(bool isWhite) {
    // Not in check  →  certainly not check-mate
    if (!isInCheck(isWhite))
        return false;

    /*  Try every legal move of every piece belonging to the side that is in
        check.  If *any* move gets us out of check, it is NOT check-mate.      */
    for (const auto& [fromPos, piecePtr] : m_board)
    {
        if (!piecePtr || piecePtr->getIsWhite() != isWhite)
            continue;

        int fromRow = fromPos.first;
        int fromCol = fromPos.second;

        for (int toRow = 0; toRow < 8; ++toRow)
        {
            for (int toCol = 0; toCol < 8; ++toCol)
            {
                // skip no-move and intra-colour captures fast
                if (fromRow == toRow && fromCol == toCol)
                    continue;

                auto destIt = m_board.find({toRow, toCol});
                if (destIt != m_board.end() && destIt->second->getIsWhite() == isWhite)
                    continue;

                /* Only analyse moves that are *legal* for that piece
                   **on the current board**.                                */
                if (!piecePtr->isValidMove(fromRow, fromCol, toRow, toCol, toString()))
                    continue;

                // Simulate and check if king still in check
                Board after = simulateMove(fromRow, fromCol, toRow, toCol);
                if (!after.isInCheck(isWhite))          // escape found
                    return false;
            }
        }
    }
    return true;   // no escape moves → check-mate
}
