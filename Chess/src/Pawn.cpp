#include "../include/Pawn.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

char Pawn::getSymbol() const {
    return isWhite ? 'P' : 'p';
}

bool Pawn::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const  string& boardStr) const {
    int dir = isWhite ? -1 : 1;  // white moves up, black down
    int startRow = isWhite ? 6 : 1;

    int rowDiff = destRow - srcRow;
    int colDiff = abs(destCol - srcCol);

    char destPiece = boardStr[destRow * 8 + destCol];

    // Regular forward move (no capture)
    if (colDiff == 0) {
        if (rowDiff == dir && destPiece == '#') return true;
        if (rowDiff == 2 * dir && srcRow == startRow &&
            boardStr[(srcRow + dir) * 8 + srcCol] == '#' &&
            destPiece == '#') return true;
    }

    // Diagonal capture
    if (colDiff == 1 && rowDiff == dir && destPiece != '#') {
        bool destIsWhite = isupper(destPiece);
        if (destIsWhite != isWhite) return true;
    }

    return false;
}
