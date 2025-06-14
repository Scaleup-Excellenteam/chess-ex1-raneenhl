//
// Created by ranee on 4/5/2025.
//

#pragma once
#include "Piece.h"
#include "Board.h"
#include <memory>


// Class for the Rook logic.
// Inherits from Piece.
class Rook : public Piece {
public:
    Rook(bool isWhite);

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const override;
    char getSymbol() const override;

    std::unique_ptr<Piece> clone() const override;

    bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const;

};

