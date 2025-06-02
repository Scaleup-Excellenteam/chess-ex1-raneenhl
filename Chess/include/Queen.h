#pragma once
#include "Piece.h"
#include <memory>


class Queen : public Piece {
public:
    Queen(bool isWhite);
    virtual ~Queen() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
    
    std::unique_ptr<Piece> clone() const override;

};
