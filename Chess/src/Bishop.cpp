#include "Bishop.h"

Bishop::Bishop(bool isWhite) : Piece(isWhite) {}

char Bishop::getSymbol() const {
    return isWhite ? 'B' : 'b';
}

/**
 * Pure geometry / path-blocking check.
 *
 * We treat the 64-char board string as
 *   boardStr[row * 8 + col]
 * with top-left = (0,0).
 */
bool Bishop::isValidMove(int srcRow, int srcCol,
                         int destRow, int destCol,
                         const std::string& boardStr) const
{
    int dRow = destRow - srcRow;
    int dCol = destCol - srcCol;

    // Bishop must move strictly diagonally
    if (std::abs(dRow) != std::abs(dCol))
        return false;

    // Determine one-square step toward destination.
    int stepRow = (dRow > 0) ? 1 : -1;
    int stepCol = (dCol > 0) ? 1 : -1;

    // Crawl square-by-square; if anything blocks, move is illegal.
    int row = srcRow + stepRow;
    int col = srcCol + stepCol;

    while (row != destRow && col != destCol) {
        if (boardStr[row * 8 + col] != '#')        // non-empty square
            return false;
        row += stepRow;
        col += stepCol;
    }

    return true;   // destination may hold opponent or be empty – handled elsewhere
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

bool Bishop::isLegalMove(int fromRow, int fromCol, int toRow,   int toCol, const Board& board) const
{
    // Currently identical to isValidMove (path check only).
    return isValidMove(fromRow, fromCol, toRow, toCol, board.toString());
}
