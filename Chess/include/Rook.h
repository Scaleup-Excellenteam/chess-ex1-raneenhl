//
// Created by ranee on 4/5/2025.
//

#pragma once
#include "Piece.h"


// Class for the Rook logic.
// Inherits from Piece.
class Rook : public Piece {
public:
    Rook(bool isWhite);

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const override;
    char getSymbol() const override;
};

