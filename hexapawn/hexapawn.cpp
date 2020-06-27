// hexapawn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <time.h>
#include <fstream>
#include <random>

#include "Move.h"

using namespace std;

struct idea
{
	Move move;
	int board[3][3];
};

void printRules();
int playGame(vector<idea>& losingMoves);
void trainAI(vector<idea>& losingMoves);

int draw(int board[3][3]);
vector<Move> calculateLegalMoves(bool isPlayerTurn, int board[3][3]);
int checkGameover(int board[3][3], vector<Move> legalMoves, bool isPlayerTurn);
Move handleInput(vector<Move> legalMoves);
Move makeCPUMove(vector<Move> legalMoves, int board[3][3], vector<idea> &losingMoves, Move lastCPUMove, int lastBoard[3][3]);
void update(int(&board)[3][3], Move move);
bool boardsEqual(int a[3][3], int b[3][3]);
void copyBoard(int(&to)[3][3], int from[3][3]);


int main()
{
	srand(time(NULL));
	cout << "James' Hexapawn game.\nType \"h\" for help, \"s\" to start a game"
		 << ", \"t\" to train the AI or \"q\" to quit\n";
	string input;
	int playerWins = 0;
	int computerWins = 0;
	int result = 0;
	vector <idea> losingMoves;

	cin >> input;
	while (input.compare("q") != 0) {

		if (input.compare("h") == 0)
		{
			printRules();
		}
		else if (input.compare("t") == 0)
		{
			trainAI(losingMoves);
		}
		else if (input.compare("s") == 0)
		{
			result = playGame(losingMoves);

			if (result == 1)
				playerWins++;
			else
				computerWins++;
			cout << "Record: " << playerWins << "W-" << computerWins << "L\n";
		}

		cout << "Type \"h\" for help, \"s\" to start a game, \"t\" to train the AI or \"q\" to quit\n";
		cin >> input;
	}
}

void printRules()
{
	std::ifstream file("rules.txt");

	if (file.is_open())
		std::cout << file.rdbuf();
	cout << "\n";
}

void trainAI(vector<idea>& losingMoves)
{
	cout << "Training...\n"; 
	int aiWinstreak = 0;
	int totalGames = 0;
	int gamesToWin = 1000;

	int lastBoardState[3][3];
	Move lastCPUMove;

	// training loop
	
	while (aiWinstreak < gamesToWin)
	{
		// The board will be represented with a 3x3 matrix.
		// -1 corresponds to 'X' pieces, 0 to empty squares. and 1 to 'O' pieces.
		// Trickily, row 0 in the matrix is actually row 3 on the board.
		// row 1 in matrix is 2 on the board, and row 2 in the matrix is row 1 on the board.
		int board[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
		// game loop
		bool isPlayerTurn = true;
		while (true)
		{
			vector <Move> legalMoves = calculateLegalMoves(isPlayerTurn, board);

			int gameOver = checkGameover(board, legalMoves, isPlayerTurn);

			if (gameOver != 0) {
				if (gameOver == 1) {
					struct idea i;
					i.move = lastCPUMove;
					copyBoard(i.board, lastBoardState);
					losingMoves.push_back(i);
					aiWinstreak = 0;
				}
				else
				{
					aiWinstreak++;
				}
				totalGames++;
				break;
			}

			Move move;

			if (isPlayerTurn)
			{
				int randNum = rand() % legalMoves.size();
				move = legalMoves[randNum];
			}
			else
			{
				move = makeCPUMove(legalMoves, board, losingMoves, lastCPUMove, lastBoardState);
				lastCPUMove = move;
				copyBoard(lastBoardState, board);
			}

			isPlayerTurn = !isPlayerTurn;
			update(board, move);
		}
	}
	cout << "AI learned by playing against random moves until it won " 
		 << gamesToWin << " games in a row, which took " << totalGames << " games.\n";
}

int playGame(vector<idea>& losingMoves)
{
	// The board will be represented with a 3x3 matrix.
	// -1 corresponds to 'X' pieces, 0 to empty squares. and 1 to 'O' pieces.
	// Trickily, row 0 in the matrix is actually row 3 on the board.
	// row 1 in matrix is 2 on the board, and row 2 in the matrix is row 1 on the board.
	int board[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	
	int lastBoardState[3][3];
	Move lastCPUMove;

	// Game loop
	int gameOver = 0;
	bool isPlayerTurn = true;
	while (!gameOver)
	{
		draw(board);

		vector <Move> legalMoves = calculateLegalMoves(isPlayerTurn, board);

		gameOver = checkGameover(board, legalMoves, isPlayerTurn);

		if (gameOver != 0) {
			if (gameOver == 1) {
				struct idea i;
				i.move = lastCPUMove;
				copyBoard(i.board, lastBoardState);
				losingMoves.push_back(i);
			}
			return gameOver;
		}

		Move move;

		if (isPlayerTurn)
		{
			move = handleInput(legalMoves);

		}
		else
		{
			move = makeCPUMove(legalMoves, board, losingMoves, lastCPUMove, lastBoardState);
			cout << "Computer move:\n";
			lastCPUMove = move;
			copyBoard(lastBoardState, board);

		}

		isPlayerTurn = !isPlayerTurn;
		update(board, move);
	}
}

int draw(int board[3][3])
{
	cout << "    A   B   C  \n  -------------\n";

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			string tile = " ";
			switch (board[row][col])
			{
			case 1:
				tile = "O";
				break;
			case -1:
				tile = "X";
				break;
			case 0:
				tile = " ";
				break;
			default:
				cerr << "\nERROR: A value in the board matrix was set to something bad!\n";
				return 1;
			}

			if (col == 0)
			{
				cout << (3 - row) << " ";
			}
			cout << "| " << tile << " ";
			if (col == 2)
			{
				cout << "|\n  -------------\n";
			}
		}
	}
	return 0;
}

vector<Move> calculateLegalMoves(bool isPlayerTurn, int board[3][3])
{
	vector<Move> moves;
	int toMove = isPlayerTurn ? 1 : -1; //the piece code for the pieces who's moves we're finding

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			// A convenience of using -1 and 1 to encode the sides, is that the tile "in front" of a piece will 
			// be the on the matrix row minus that piece's side number. we can also easily get the code for the enemy pieces
			// of whichever code is in toMove by flipping the sign of toMove.

			if (row - toMove != -1 && row - toMove != 3 && board[row][col] == toMove) {

				// if there is an empty tile in front of the piece, create a move corresponding to the piece there.
				if (board[row - toMove][col] == 0) {

					array<int, 2> fromCoord = { row, col };
					array<int, 2> toCoord = { row - toMove, col };

					Move m = Move(fromCoord, toCoord, toMove);
					moves.push_back(m);
				}
				// if there is an enemy pawn on the right diagonal in front of the piece, create a move for taking that piece.
				if (col + 1 != 3 && board[row - toMove][col + 1] == -(toMove)) {

					array<int, 2> fromCoord = { row, col };
					array<int, 2> toCoord = { row - toMove, col + 1 };

					Move m = Move(fromCoord, toCoord, toMove);
					moves.push_back(m);
				}
				// if there is an enemy pawn on the left diagonal in front of the piece, create a move for taking that piece.
				if (col - 1 != -1 && board[row - toMove][col - 1] == -(toMove)) {

					array<int, 2> fromCoord = { row, col };
					array<int, 2> toCoord = { row - toMove, col - 1 };

					Move m = Move(fromCoord, toCoord, toMove);
					moves.push_back(m);
				}
			}
		}
	}

	return moves;
}

/*
	checkGameOver:
	return value: returns 1 if player won, returns -1 if computer won, returns 0 if game isn't over.
*/
int checkGameover(int board[3][3], vector<Move> legalMoves, bool isPlayerTurn)
{
	//check for pawns that got to the other end of the board and count the number of pieces on each side.
	int countX = 0;
	int countO = 0;
	for (int col = 0; col < 3; col++) {
		if (board[0][col] == 1)
		{
			cout << "You got a piece to the other end of the board, you win!\n";
			return 1;
		}
		if (board[2][col] == -1)
		{
			cout << "Computer got a piece to the other end of the board, computer wins!\n";
			return -1;
		}
		for (int row = 0; row < 3; row++)
		{
			if (board[row][col] == -1)
				countX++;
			if (board[row][col] == 1)
				countO++;
		}
	}
	// If one side had all pieces captured, they lost.
	if (countX == 0) {
		cout << "You win!\n";
		return 1;
	}
	else if (countO == 0) {
		cout << "Computer wins!\n";
		return -1;
	}

	if (legalMoves.size() == 0) {
		if (isPlayerTurn)
		{
			cout << "You have no legal moves, computer wins!\n";
			return -1;
		}
		else
		{
			cout << "Computer has no legal moves, you win!\n";
			return 1;
		}
	}

	return 0;
}

Move handleInput(vector<Move> legalMoves)
{
	while (true) {

		cout << "Enter a move: ";
		string input;
		cin >> input;

		if (input.size() == 5 && (input[2] == '-')
			&& ((input[0] >= 'a' && input[0] <= 'c') || (input[0] >= 'A' && input[0] <= 'C'))
			&& ((input[3] >= 'a' && input[3] <= 'c') || (input[3] >= 'A' && input[3] <= 'C'))
			&& (input[1] >= '1' && input[1] <= '3')
			&& (input[4] >= '1' && input[4] <= '3')
			)
		{
			// Converting the input coords to work with our matrix.
			// Chess coords actually list the column first but column
			// comes second in our matrix coords, so keep that in mind.
			array<int, 2> fromCoord;
			array<int, 2> toCoord;
			if (input[0] == 'a' || input[0] == 'A')
			{
				fromCoord[1] = 0;
			}
			else if (input[0] == 'b' || input[0] == 'B')
			{
				fromCoord[1] = 1;
			}
			else if (input[0] == 'c' || input[0] == 'C')
			{
				fromCoord[1] = 2;
			}
			fromCoord[0] = 3 - ((int)input[1] - 48); // ascii conversion, and converting from board coord to matrix coord

			if (input[3] == 'a' || input[3] == 'A')
			{
				toCoord[1] = 0;
			}
			else if (input[3] == 'b' || input[0] == 'B')
			{
				toCoord[1] = 1;
			}
			else if (input[3] == 'c' || input[0] == 'C')
			{
				toCoord[1] = 2;
			}
			toCoord[0] = 3 - ((int)input[4] - 48); // ascii conversion, and converting from board coord to matrix coord

			Move m = Move(fromCoord, toCoord, 1);

			for (unsigned int i = 0; i < legalMoves.size(); i++)
			{
				if (m.equals(legalMoves[i])) {
					return m;
				}

			}
			cout << "Move is not available\n";
			//TODO: print out valid moves to help user
		}
		else
		{
			cout << "Move is not in valid format\nPlease format your move as [starting coordinate]-[coordinate to move to]\n";
		}
	}
	return Move();
}

/*
This method's logic is really confusing and messy, could probably be cleaned up
 */
Move makeCPUMove(vector<Move> legalMoves, int board[3][3], vector<idea>& losingMoves, Move lastCPUMove, int lastBoard[3][3])
{
	//srand (time(NULL));

	// Randomize the order we check moves.
	random_shuffle(legalMoves.begin(), legalMoves.end());

	// pick the first move and assume it is bad
	bool isLosingMove = true;
	int i = 0;
	Move move = legalMoves[i];
	
	// while the move is bad
	while (isLosingMove)
	{
		if (losingMoves.size() == 0)
		{
			isLosingMove = false;
		}
		// go through each element in losing moves
		for (int j = 0; j < losingMoves.size(); j++)
		{
			// if our move matches an element from losingmoves, 
			if (move.equals(losingMoves[j].move) && boardsEqual(board, losingMoves[j].board))
			{
				// If we failed to find a legal move that isn't losing, then the LAST move the CPU did must be added to losing moves
				if (i == legalMoves.size() - 1)
				{
					struct idea i;
					i.move = lastCPUMove;
					copyBoard(i.board, lastBoard);
					losingMoves.push_back(i);

					// play the losing move, we have no choice!
					return move;

				}
				// if legal move is losing, but we can still check more legal moves, move on to the next one
				else
				{
					isLosingMove = true;
					i++;
					move = legalMoves[i];
					break;
				}
			}
			else
			{
				isLosingMove = false;
			}
		}
	}
	return move;
}

void update(int(&board)[3][3], Move move)
{
	array<int, 2> fromCoord = move.getFromCoord();
	array<int, 2> toCoord = move.getToCoord();

	board[fromCoord[0]][fromCoord[1]] = 0;
	board[toCoord[0]][toCoord[1]] = move.getPiece();
}

bool boardsEqual(int a[3][3], int b[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (a[i][j] != b[i][j])
				return false;
		}
	}
	return true;
}

void copyBoard(int (&to)[3][3], int from[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}