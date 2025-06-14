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

  /**
   * Validates a move from standard notation (e.g., "e2e4").
   * @return int - Code indicating result:
   * 11 - No piece at source
   * 12 - Not your piece
   * 13 - Destination occupied
   * 21 - Invalid movement
   * 31 - Would result in check
   * 41 - Legal move, causes check
   * 42 - Legal move, normal
  */
  int validateMove(const string& input); // returns codeResponse
  
  bool isKingInCheck(bool whiteKing) const;
  string toString() const;
  
  /**
 * Returns true if the player has any legal move that doesn't result in check.
 */
  bool hasAnyLegalMove(bool forWhite);
  static pair<int, int> parsePosition(char file, char rank);
  char getPieceSymbol(int row, int col) const;
  Piece* getPiece(int row, int col) const;
  void movePiece(int fromRow, int fromCol, int toRow, int toCol);
  
  /**
 * Returns a copy of the board after applying the move.
 * Does not affect the current board.
 * Used for prediction/evaluation.
 */
  Board simulateMove(int fromRow, int fromCol, int toRow, int toCol) const;
  const std::map<std::pair<int, int>, std::unique_ptr<Piece>>& getPieces() const;
  bool isInCheck(bool isWhite) const; 
  bool isInCheckmate (bool isWhite);
  
  // Lightweight check to validate if a move is legal for the current board state
  bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) const;
  bool isInBounds(int row, int col) const;

  void setTurn(bool white);
  bool getTurn() const; 

private:
  map<pair<int, int>, unique_ptr<Piece>> m_board;
  bool m_turnWhite = true;
  void parseBoardString(const string& boardString);

};

