// Chess 
#include "Chess.h"
#include "Board.h"
#include <MoveRecommender.h>

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

		/**/ 
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
	delete boardEngine;

	cout << endl << "Exiting " << endl; 
	return 0;
}