#pragma once
#include "Piece.h"
#include "Board.h"
#include <memory>

// Represents a queen piece. Combines rook and bishop movement logic
class Queen : public Piece {
public:
    Queen(bool isWhite);
    virtual ~Queen() = default;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const override;
    char getSymbol() const override;
    
    std::unique_ptr<Piece> clone() const override;

    bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const;

};
