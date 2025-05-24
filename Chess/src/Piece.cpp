//
// Created by ranee on 4/5/2025.
//

#include <cctype>
#include "Piece.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Knight.h"

std::string Piece::simulateMove(int srcRow, int srcCol, int destRow, int destCol, const std::string& board) {
    std::string newBoard = board;
    newBoard[destRow * 8 + destCol] = board[srcRow * 8 + srcCol];
    newBoard[srcRow * 8 + srcCol] = '#';
    return newBoard;
}

bool Piece::isKingInCheck(bool white, const std::string& board) {
    int kingRow = -1, kingCol = -1;
    char kingSymbol = white ? 'K' : 'k';

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r * 8 + c] == kingSymbol) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char ch = board[r * 8 + c];
            if (ch == '#' || (white && isupper(ch)) || (!white && islower(ch)))
                continue;

            Piece* p = Piece::createPiece(ch);
            if (p && p->isValidMove(r, c, kingRow, kingCol, board)) {
                delete p;
                return true;
            }
            delete p;
        }
    }

    return false;
}

Piece* Piece::createPiece(char symbol) {
    bool isWhite = isupper(symbol);
    switch (tolower(symbol)) {
        case 'r': return new Rook(isWhite);
        case 'n': return new Knight(isWhite);
        case 'b': return new Bishop(isWhite);
        case 'q': return new Queen(isWhite);
        case 'k': return new King(isWhite);
        case 'p': return new Pawn(isWhite);
        default: return nullptr;
    }
}


