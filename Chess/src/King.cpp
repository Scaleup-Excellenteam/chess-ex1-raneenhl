
#include "../include/King.h"

King::King(bool isWhite) : Piece(isWhite) {}

char King::getSymbol() const {
    return isWhite ? 'K' : 'k';
}

bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const {
return abs(srcRow - destRow) <= 1 && abs(srcCol - destCol) <= 1;
}

std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}

bool King::isLegalMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    return isValidMove(fromRow, fromCol, toRow, toCol, board.toString());
}
