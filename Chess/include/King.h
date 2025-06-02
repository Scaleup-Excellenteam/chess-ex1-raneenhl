//
// Created by ranee on 4/5/2025.
//

#pragma once
#include "Piece.h"
#include <string>
#include <memory>

using namespace std;



class King : public Piece {
public:
  King(bool isWhite);

  bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const override;
  char getSymbol() const override;

  std::unique_ptr<Piece> clone() const override;

};


