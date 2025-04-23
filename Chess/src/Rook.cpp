//
// Created by ranee on 4/5/2025.
//

#include "../include/Rook.h"

Rook::Rook(bool isWhite) : Piece(isWhite) {}

char Rook::getSymbol() const {
    return isWhite ? 'R' : 'r';
}

bool Rook::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const  string& boardStr) const {
    if (fromRow != toRow && fromCol != toCol)
        return false; // not straight line

    int stepRow = (toRow > fromRow) - (toRow < fromRow); // 1, 0 or -1
    int stepCol = (toCol > fromCol) - (toCol < fromCol);

    int row = fromRow + stepRow;
    int col = fromCol + stepCol;

    while (row != toRow || col != toCol) {
        char curr = boardStr[row * 8 + col];
        if (curr != '#')
            return false; // blocked
        row += stepRow;
        col += stepCol;
    }

    return true;
}
