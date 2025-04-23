#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool isWhite);
    virtual ~Pawn() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
};
