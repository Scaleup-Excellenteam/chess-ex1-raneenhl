#include "Bishop.h"

Bishop::Bishop(bool isWhite) : Piece(isWhite) {}

char Bishop::getSymbol() const {
    return isWhite ? 'B' : 'b';
}

bool Bishop::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const {
    int dRow = destRow - srcRow;
    int dCol = destCol - srcCol;

    // Move must be diagonal
    if (abs(dRow) != abs(dCol)) return false;

    int stepRow = (dRow > 0) ? 1 : -1;
    int stepCol = (dCol > 0) ? 1 : -1;

    int row = srcRow + stepRow;
    int col = srcCol + stepCol;

    while (row != destRow && col != destCol) {
        if (boardStr[row * 8 + col] != '#')
            return false; // path blocked
        row += stepRow;
        col += stepCol;
    }

    return true;
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}