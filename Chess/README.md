# Chess Game Project

## Compilation and Execution

To compile the project, navigate to the root directory and use CMake:

```bash
mkdir build
cd build
cmake ..
make
./Chess
```

## Project Structure

```
Chess/
├── include/           # Header files for all pieces and board
├── src/               # Implementation files for game logic and AI
├── main.cpp           # Main execution file
├── CMakeLists.txt     # CMake configuration
├── README.md          # This file
```

## Rules Implemented

- All basic movement rules for each piece.
- Check and checkmate detection.
- Castling (both short and long).
- Pawn promotion to queen.
- Turn-based enforcement (white and black alternate).
- Prevents illegal moves and own-piece captures.

## Extra Features

- AI opponent using Minimax-like recommendation engine.
- Castling notation support (`O-O` and `O-O-O`).
- Basic multithreaded benchmarking support.
- Stalemate detection (draw when no legal moves and not in check).

## AI Algorithm

The AI uses a recursive move evaluation algorithm inspired by Minimax with a scoring system:
- Prioritizes captures, threats, and safety.
- Penalizes exposure to weaker threats.
- Considers castling as a positive move.

We chose this design for its balance of performance and strategic evaluation.

## Code Testing

- Validated each rule using scenarios: check, checkmate, promotion, castling.
- Edge cases tested: empty move, invalid length, blocked paths, double move.
- Multiple playthroughs with and without AI to ensure stability.

## Edge Cases Handled

- Invalid inputs and length.
- Player tries to move opponent's piece.
- Moves causing self-check.
- No legal moves → checkmate or stalemate.

## Challenges and Improvements

- Debugging AI scoring to prevent stuck behavior.
- Balancing move recommendation depth vs. performance.
- If there was more time, I’d add: move history, undo, save/load, full GUI.