#pragma once
#include <array>

using namespace std;

class Move
{
public:
	Move();
	~Move();
	Move(array<int, 2> f, array<int, 2> t, int p);
	
	array<int, 2> getFromCoord();
	array<int, 2> getToCoord();
	int getPiece();

	bool equals(Move m);

private:
	array<int, 2> fromCoord;
	array<int, 2> toCoord;
	int piece;
};