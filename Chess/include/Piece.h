//
// Created by ranee on 4/5/2025.
//

#pragma once
#include <string>
#include <memory>


using namespace std;

class Board;

// Abstract base class for all pieces
class Piece {

public:
  Piece(bool isWhite) : isWhite(isWhite) {}
  virtual ~Piece() = default;

  bool getIsWhite() const { return isWhite; }

  // Validate move based on type
  // Returns whether a move is valid for the piece, based only on piece rules and board string (not legality like check).
  virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const string& boardStr) const = 0;
  virtual char getSymbol() const = 0;

  static bool isKingInCheck(bool white, const std::string& board);

  // Returns the new board string after making a hypothetical move
  static string simulateMove(int srcRow, int srcCol, int destRow, int destCol, const std::string& board);

  // Factory method to construct a piece object from its character symbol
  static Piece* createPiece(char symbol);

  virtual std::unique_ptr<Piece> clone() const = 0;

  void setHasMoved(bool moved) { hasMoved = moved; }
  bool getHasMoved() const { return hasMoved; }

  // Full legality check including board state
  virtual bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const = 0;


protected:
  bool isWhite;
  bool hasMoved = false;
};

