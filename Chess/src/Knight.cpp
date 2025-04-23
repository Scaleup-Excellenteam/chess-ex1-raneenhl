#include "../include/Knight.h"

Knight::Knight(bool isWhite) : Piece(isWhite) {}

char Knight::getSymbol() const {
    return isWhite ? 'N' : 'n';
}

bool Knight::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const {
    int dRow = abs(destRow - srcRow);
    int dCol = abs(destCol - srcCol);
    return (dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2);
}