#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    explicit Knight(bool isWhite);

    char getSymbol() const override;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const std::string& boardStr) const override;
};

#endif // KNIGHT_H
