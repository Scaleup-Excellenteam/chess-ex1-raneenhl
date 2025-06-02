// Chess 
#include "Chess.h"
#include "Board.h"
#include "MoveRecommender.h"
#include <chrono>

void runBenchmarks(Board& board, int depth);

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
//	string board = "##########K###############################R#############r#r#####";
	Chess a(board);
	int codeResponse = 0;
	string res = a.getInput();

	Board* boardEngine = new Board(board, true);
    bool whiteTurn = true;

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

		codeResponse = boardEngine->validateMove(res);

		// If move was valid, update turn
        if (codeResponse == 41 || codeResponse == 42)
        {
            int fromRow = res[0] - 'a';
            int fromCol = res[1] - '1';
            int toRow = res[2] - 'a';
            int toCol = res[3] - '1';

            Board* newBoard = new Board(boardEngine->simulateMove(fromRow, fromCol, toRow, toCol));
            delete boardEngine;
            boardEngine = newBoard;
            whiteTurn = !whiteTurn;
		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput(); 

		if (res != "exit")
        {
            MoveRecommender recommender(*boardEngine, 2);
            recommender.calculateMoves(whiteTurn, 3);
            cout << "\nTop recommended moves:\n";
            cout << recommender << endl;
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