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
  ~Board();
  int validateMove(const string& input); // returns codeResponse
  bool isKingInCheck(bool whiteKing) const;
  string toString() const;
  bool hasAnyLegalMove(bool forWhite);
  static pair<int, int> parsePosition(char file, char rank);

private:
  map<pair<int, int>, unique_ptr<Piece>> m_board;
  bool m_turnWhite;
  void parseBoardString(const string& boardString);

};

