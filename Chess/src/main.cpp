// Chess 
#include "Chess.h"
#include "Board.h"
#include "MoveRecommender.h"
#include <chrono>
#include <iostream>

void runBenchmarks(Board& board, int depth);

static std::string normaliseCastling(const std::string& in, bool whiteTurn) {
    if (in == "O-O"   || in == "o-o")   return whiteTurn ? "e1g1" : "e8g8";
    if (in == "O-O-O" || in == "o-o-o") return whiteTurn ? "e1c1" : "e8c8";
    return in;
}

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
//	string board = "##########K###############################R#############r#r#####";

    // Step 1: Ask the player which mode to play
    bool playAgainstAI = false;
        std::cout << "Select game mode:\n"
                 "1. Player vs Player\n"
                 "2. Player vs Computer (AI)\n"
                 "Enter choice (1 or 2): ";
    int mode;
    std::cin >> mode;

    if (mode == 2) {
        playAgainstAI = true;
        std::cout << "You are playing against the computer.\n";
    }
    
    // initial objects
	Chess a(board);
    Board* boardEngine = new Board(board, true);
    bool whiteTurn = true;
    boardEngine->setTurn(whiteTurn);       // keep Board in sync
    a.setTurn(whiteTurn);  

    // first prompt
    int codeResponse = 0;
    std::string res = normaliseCastling(a.getInput(), whiteTurn);
    //std::string res = a.getInput();

    // main loop
	while (res != "exit")
	{
		/* 
		codeResponse value : 
		Illegal movements : 
		11 - there is not piece at the source  
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination 
		21 - illegal movement of that piece 
		31 - this movement will cause you checkmate

		legal movements : 
		41 - the last movement was legal and cause check 
		42 - the last movement was legal, next turn 
		*/
        
        // validate
		codeResponse = boardEngine->validateMove(res);

		// If move was valid, update turn
        if (codeResponse == 41 || codeResponse == 42) {
            // execute on a fresh copy
            auto [fromRow, fromCol] = Board::parsePosition(res[0], res[1]);
            auto [toRow, toCol] = Board::parsePosition(res[2], res[3]);

            Board* newBoard = new Board(boardEngine->simulateMove(fromRow, fromCol, toRow, toCol));
            delete boardEngine;
            boardEngine = newBoard;

            // castling message (once)
            bool castled = (res == "e1g1" || res == "e1c1" ||
                            res == "e8g8" || res == "e8c8");
            if (castled && codeResponse == 42)
                std::cout << "Castling move executed!\n";

            // check-mate? 
            if (boardEngine->isInCheckmate(!whiteTurn))
            {
                a.setCodeResponse(51);   // checkmate
                std::cout << "\nCheck-mate! "
                          << (whiteTurn ? "White" : "Black")
                          << " wins.\n";
                break;
            }

            // stalemate check
            if (!boardEngine->isKingInCheck(!whiteTurn) && !boardEngine->hasAnyLegalMove(!whiteTurn)) {
                a.setCodeResponse(52);  // draw
                std::cout << "\nStalemate! The game is a draw.\n";
                break;
            }

            // switch turn 
            whiteTurn = !whiteTurn;
            boardEngine->setTurn(whiteTurn);   // sync
            a.setTurn(whiteTurn);  

		}

		a.setCodeResponse(codeResponse);
    
        // AI turn
        if (playAgainstAI && !whiteTurn)
        {
            MoveRecommender recommender(*boardEngine, 2);

            recommender.calculateMoves(false, 5);

            if (recommender.getTopMoves().empty()) {
                std::cout << "AI has no legal moves.\n";
                break;
            }

            bool validAIMove = false;
            auto aiMoves = recommender.getTopMovesCopy();
            for (const auto& move : aiMoves.snapshot()) {
                std::cout << move.notation << " ";
            }
            std::cout << "\n";

            while (!aiMoves.empty()) {
                std::string best = aiMoves.top().notation;
                aiMoves.poll();

                int code = boardEngine->validateMove(best);
                if (code == 41 || code == 42) {
                    std::cout << "\nAI plays: " << best << '\n';
                    auto [fr, fc] = Board::parsePosition(best[0], best[1]);
                    auto [tr, tc] = Board::parsePosition(best[2], best[3]);

                    Board* next   = new Board(boardEngine->simulateMove(fr, fc, tr, tc));
                    delete boardEngine;
                    boardEngine   = next;

                    // castling message for AI 
                    bool aiCastled = (best == "e8g8" || best == "e8c8");
                    if (aiCastled)
                        std::cout << "AI performed castling.\n";

                    whiteTurn = true;             // back to human
                    boardEngine->setTurn(whiteTurn);
                    a.setTurn(whiteTurn);  
                    a.setCodeResponse(code);
                
                    validAIMove = true;
                    break;
                }
            }
            if (!validAIMove) {
                std::cout << "AI failed to find a valid move.\n";
                break;
            }

            // next human input */
            res = normaliseCastling(a.getInput(), whiteTurn);
            continue;
        }

        // human next input
        if (res != "exit")
        {
            MoveRecommender recommender(*boardEngine, 2);
            recommender.calculateMoves(whiteTurn, 3);
            cout << "\nTop recommended moves:\n" << recommender << endl;

            res = normaliseCastling(a.getInput(), whiteTurn); 
        }
        
	}

	// Benchmark on final board state
    int depth = 2;
    std::cout << "\nRunning benchmarks...\n";
    runBenchmarks(*boardEngine, depth);

    delete boardEngine;
    std::cout << "\nExiting\n";

	return 0;
}


void runBenchmarks(Board& board, int depth) {
    std::vector<int> threadCounts = {0, 2, 4, 8};

    for (int threads : threadCounts) {
        MoveRecommender recommender(board, depth);
        auto start = std::chrono::high_resolution_clock::now();

        if (threads == 0) {
            // Single-threaded fallback (original)
            recommender.calculateMovesSingleThreaded(true, 8);
        } else {
            recommender.calculateMoves(true, 8);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Threads: " << threads
                  << " — Time: " << duration.count() << " seconds" << std::endl;
    }
}