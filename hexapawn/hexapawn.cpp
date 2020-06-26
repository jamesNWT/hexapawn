// hexapawn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <time.h>

using namespace std;

class Move {
private:
	array<int, 2> fromCoord;
	array<int, 2> toCoord;
	int piece;

public:
	Move() {}

	Move(array<int, 2> f, array<int, 2> t, int p)
		: fromCoord(f), toCoord(t), piece(p)
	{}

	array<int, 2> getFromCoord() { return this->fromCoord; }
	array<int, 2> getToCoord() { return this->toCoord; }
	int getPiece() { return this->piece;  }

	bool equals(Move m)
	{
		return (this->fromCoord == m.fromCoord) && (this->toCoord == m.toCoord);
	}
};

void printRules();
void playGame();

int draw(int board[3][3]);
vector<Move> calculateLegalMoves(bool isPlayerTurn, int board[3][3]);
bool checkGameover(int board[3][3], vector<Move> legalMoves, bool isPlayerTurn);
Move handleInput(vector<Move> legalMoves);
Move makeCPUMove(vector<Move> legalMoves);
void update(int (&board)[3][3], Move move);


int main()
{
    cout << "James' Hexapawn game.\nType \"r\" for rules, \"s\" to start a game, and \"q\" to quit\n";
	string input;
	
	cin >> input;
	while (input.compare("q") != 0) {
	
		if (input.compare("r") == 0)
		{
			printRules();
		}
		else if (input.compare("s") == 0)
		{
			playGame();
		}
	
		cout << "Type \"r\" for rules, \"s\" to start a game, and \"q\" to quit\n";
		cin >> input;
	}

}

void printRules()
{
	cout << "I'll write up the rules later\n";
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
		if (isPlayerTurn)
			draw(board);
		
		vector <Move> legalMoves = calculateLegalMoves(isPlayerTurn, board);

		gameOver = checkGameover(board, legalMoves, isPlayerTurn);

		if (gameOver) {
			draw(board);
			break;
		}

		Move move;
		
		if (isPlayerTurn)
		{
			move = handleInput(legalMoves);

		}
		else
		{
			move = makeCPUMove(legalMoves);
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
				if (col+1 != 3 && board[row - toMove][col + 1] == -(toMove)) {
					
					array<int, 2> fromCoord = { row, col };
					array<int, 2> toCoord = { row - toMove, col + 1};

					Move m = Move(fromCoord, toCoord, toMove);
					moves.push_back(m);
				}
				// if there is an enemy pawn on the left diagonal in front of the piece, create a move for taking that piece.
				if (col-1 != -1 && board[row - toMove][col - 1] == -(toMove)) {

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

bool checkGameover(int board[3][3], vector<Move> legalMoves, bool isPlayerTurn)
{
	//check for pawns that got to the other end of the board and count the number of pieces on each side.
	int countX = 0;
	int countO = 0;
	for (int col = 0; col < 3; col++) {
		if (board[0][col] == 1)
		{
			cout << "You got a piece to the other end of the board, you win!\n";
			return true;
		}
		if (board[2][col] == -1)
		{
			cout << "Computer got a piece to the other end of the board, computer wins!\n";
			return true;
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
		return true;
	}
	else if (countO == 0) {
		cout << "Computer wins!";
		return true;
	}

	if (legalMoves.size() == 0) {
		if (isPlayerTurn)
			cout << "You have no legal moves, computer wins!\n";
		else
			cout << "Computer has no legal moves, you win!\n";
		return true;
	}

	return false;
}

Move handleInput(vector<Move> legalMoves)
{
	while(true){

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

//TODO: implement some AI here.
Move makeCPUMove(vector<Move> legalMoves)
{
	srand (time(NULL));
	int i = (rand() % legalMoves.size());

	return legalMoves[i];
}

void update(int(&board)[3][3], Move move)
{
	array<int, 2> fromCoord = move.getFromCoord();
	array<int, 2> toCoord = move.getToCoord();

	board[fromCoord[0]][fromCoord[1]] = 0;
	board[toCoord[0]][toCoord[1]] = move.getPiece();


}
