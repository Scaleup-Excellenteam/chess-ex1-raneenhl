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

std::unique_ptr<Piece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

bool Knight::isLegalMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    return isValidMove(fromRow, fromCol, toRow, toCol, board.toString());
}
