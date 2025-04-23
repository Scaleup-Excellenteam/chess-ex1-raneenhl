
#include "../include/King.h"

King::King(bool isWhite) : Piece(isWhite) {}

char King::getSymbol() const {
    return isWhite ? 'K' : 'k';
}

bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const {
return abs(srcRow - destRow) <= 1 && abs(srcCol - destCol) <= 1;
}
