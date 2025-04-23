#include "../include/Queen.h"

Queen::Queen(bool isWhite) : Piece(isWhite) {}

char Queen::getSymbol() const {
    return isWhite ? 'Q' : 'q';
}

bool Queen::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const {
    int dRow = destRow - srcRow;
    int dCol = destCol - srcCol;

    // Either straight (like rook) or diagonal (like bishop)
    if (srcRow == destRow || srcCol == destCol || abs(dRow) == abs(dCol)) {
        int stepRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
        int stepCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);

        int row = srcRow + stepRow;
        int col = srcCol + stepCol;

        while (row != destRow || col != destCol) {
            if (boardStr[row * 8 + col] != '#')
                return false; // blocked
            row += stepRow;
            col += stepCol;
        }
        return true;
    }

    return false; // invalid queen move
}
