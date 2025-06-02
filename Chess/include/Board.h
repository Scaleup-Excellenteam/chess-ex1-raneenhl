//
// Created by ranee on 4/5/2025.
//

#pragma once

#include <memory>
#include <map>
#include <string>
#include "Piece.h"



/*
    Main engine class to:
    Load and interpret m_boardString.
    Manage Piece* instances.
    Check move validity.
    Return the correct codeResponse
*/

class Board {

public:
  Board();
  Board(const  string& boardString, bool isWhiteTurn);
  Board(const Board& other);
  ~Board();
  Board& operator=(Board&& other) noexcept = default;
  Board& operator=(const Board&) = delete;
  int validateMove(const string& input); // returns codeResponse
  bool isKingInCheck(bool whiteKing) const;
  string toString() const;
  bool hasAnyLegalMove(bool forWhite);
  static pair<int, int> parsePosition(char file, char rank);
  char getPieceSymbol(int row, int col) const;
  Piece* getPiece(int row, int col) const;
  void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  Board simulateMove(int fromRow, int fromCol, int toRow, int toCol) const;
  const std::map<std::pair<int, int>, std::unique_ptr<Piece>>& getPieces() const;


private:
  map<pair<int, int>, unique_ptr<Piece>> m_board;
  bool m_turnWhite;
  void parseBoardString(const string& boardString);

};

