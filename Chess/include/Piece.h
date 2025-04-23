//
// Created by ranee on 4/5/2025.
//

#pragma once
#include <string>

using namespace std;


// Abstract base class for all pieces
class Piece {

public:
  Piece(bool isWhite) : isWhite(isWhite) {}
  virtual ~Piece() = default;

  bool getIsWhite() const { return isWhite; }

  // Validate move based on type
  virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const = 0;
  virtual char getSymbol() const = 0;

protected:
  bool isWhite;
};

