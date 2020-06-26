#pragma once
#include <array>
#include "Move.h"

using namespace std;

Move::Move() {}
Move::~Move() {}

Move::Move(array<int, 2> f, array<int, 2> t, int p)
	: fromCoord(f), toCoord(t), piece(p)
{}

array<int, 2> Move::getFromCoord() { return this->fromCoord; }
array<int, 2> Move::getToCoord() { return this->toCoord; }
int Move::getPiece() { return this->piece; }

bool Move::equals(Move m)
{
	return (this->fromCoord == m.fromCoord) && (this->toCoord == m.toCoord);
}

