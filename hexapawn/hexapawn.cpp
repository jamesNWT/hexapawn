// hexapawn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

class Move {
private:
	array<int, 2> fromCoord;
	array<int, 2> toCoord;

public:
	Move() {}

	Move(array<int, 2> f, array<int, 2> t)
		: fromCoord(f), toCoord(t)
	{}

	bool equals(Move m)
	{
		return (this->fromCoord == m.fromCoord) && (this->toCoord == m.toCoord);
	}
};

void printRules();
void playGame();

void draw(int board[3][3]);
vector<Move> calculateLegalMoves(bool isPlayerTurn, int board[3][3]);
bool checkGameover(int board[3][3], vector<Move> legalMoves);
Move handleInput(vector<Move> legalMoves);
Move makeCPUMove(vector<Move> legalMoves);
void update(int (&board)[3][3], Move move);


int main()
{
    cout << "James' Hexapawn game.\nType \"r\" for rules, \"s\" to start a game, and \"q\" to quit\n";
	string input;
	cin >> input;
	
	if (input.compare("r") == 0)
	{
		printRules();
	}
	else if (input.compare("s") == 0)
	{
		playGame();
	}
	else
	{
		cout << "Type \"r\" for rules, \"s\" to start a game, and \"q\" to quit\n";
	}

}

void printRules()
{
	cout << "I'll write up the rules later";
}

void playGame()
{
	// The board will be represented with a 3x3 matrix.
	// -1 corresponds to 'X' pieces, 0 to empty squares. and 1 to 'O' pieces.
	// Trickily, row 0 in the matrix is actually row 3 on the board.
	// row 1 in matrix is 2 on the board, and row 2 in the matrix is row 1 on the board.
	int board[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };

	// Game loop
	bool gameOver = false;
	bool isPlayerTurn = true;
	while (!gameOver)
	{
		draw(board);
		
		vector <Move> legalMoves;

		legalMoves = calculateLegalMoves(isPlayerTurn, board);

		gameOver = checkGameover(board, legalMoves);

		Move move;
		
		if (isPlayerTurn)
		{
			move = handleInput(legalMoves);

		}
		else
		{
			move = makeCPUMove(legalMoves);
		}
		
		update(board, move);
	}
}

void draw(int board[3][3])
{
}

vector<Move> calculateLegalMoves(bool isPlayerTurn, int board[3][3])
{
	return vector<Move>();
}

bool checkGameover(int board[3][3], vector<Move> legalMoves)
{
	return false;
}

Move handleInput(vector<Move> legalMoves)
{
	return Move();
}

Move makeCPUMove(vector<Move> legalMoves)
{
	return Move();
}

void update(int(&board)[3][3], Move move)
{
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
