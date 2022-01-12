#pragma once
#include "Piece.h"
#include "Camera.h"

class Queen :public Piece {
public:
	Queen(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Queen/Queen.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {};
};