#pragma once
#include "Piece.h"
#include "Camera.h"

class Pawn :public Piece {
public:
	Pawn(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Pawn/Pawn.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {};
};