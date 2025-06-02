#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <memory>


class Knight : public Piece {
public:
    explicit Knight(bool isWhite);

    char getSymbol() const override;

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const std::string& boardStr) const override;

    std::unique_ptr<Piece> clone() const override;

};

#endif // KNIGHT_H
