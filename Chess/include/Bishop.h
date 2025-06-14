#pragma once
#include "Piece.h"
#include "Board.h"

/**
 * @brief Concrete chess-piece class representing a Bishop.
 *
 *  - Moves diagonally any distance as long as no piece blocks the path.
 *  - Inherits colour, has-moved flag, etc. from Piece.
 */
class Bishop : public Piece {
public:
    /**
     * Construct a bishop of the given colour.
     * @param isWhite  true  → white bishop  |  false → black bishop
     */
    explicit Bishop(bool isWhite);
    ~Bishop() override = default;

    /**
     * Validates bishop movement on a raw board-string view.
     * Pure geometric check; does not look at check / turn legality.
     */
    bool isValidMove(int srcRow, int srcCol,
                     int destRow, int destCol,
                     const std::string& boardStr) const override;

    /**
     * @return  single-char representation  ( 'B' / 'b' )
     */
    char getSymbol() const override;

    /**
     * Polymorphic deep-copy helper.
     */
    std::unique_ptr<Piece> clone() const override;

    /**
     * Full *legal* verification that also consults Board state.
     * (Currently just forwards to isValidMove, but kept for symmetry with other pieces.)
     */
    bool isLegalMove(int fromRow, int fromCol,
                     int toRow,   int toCol,
                     const Board& board) const;
};
