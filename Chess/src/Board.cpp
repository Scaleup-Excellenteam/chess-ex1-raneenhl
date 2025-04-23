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

void Board::parseBoardString(const string& boardString) {
    m_board.clear();
    for (int i = 0; i < 64; ++i) {
        char c = boardString[i];
        if (c == '#') continue;

        bool isWhite = isupper(c);
        int row = i / 8;
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

    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, toString()))
        return 21;  // Invalid move for that piece

    // Simulate move
    auto movedPiece = move(m_board[{fromRow, fromCol}]);
    auto capturedPiece = m_board.count({toRow, toCol}) ? move(m_board[{toRow, toCol}]) : nullptr;

    m_board[{toRow, toCol}] = move(movedPiece);
    m_board.erase({fromRow, fromCol});

    // Check if this move leaves your own king in check
    if (isKingInCheck(m_turnWhite)) {
        // Undo move
        m_board[{fromRow, fromCol}] =  move(m_board[{toRow, toCol}]);
        if (capturedPiece)
            m_board[{toRow, toCol}] =  move(capturedPiece);
        else
            m_board.erase({toRow, toCol});

        return 31;  // Move exposes own king to check
    }

    // Check if opponent is in check after the move
    if (isKingInCheck(!m_turnWhite)) {
        if (!hasAnyLegalMove(!m_turnWhite)) {
            cout << "Checkmate! Game Over.\n";
            return 41;  // Checkmate (also treated as check)
        }
        return 41;  // Just check
    }

    return 42;  // Legal move, no check
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
        int row = pos.first;
        int col = pos.second;
        result[row * 8 + col] = piece->getSymbol();
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
