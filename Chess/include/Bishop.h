#pragma once
#include "Piece.h"


class Bishop : public Piece {
public:
    Bishop(bool isWhite);
    virtual ~Bishop() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
};
