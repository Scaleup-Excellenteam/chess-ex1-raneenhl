#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite);
    virtual ~Queen() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
};
