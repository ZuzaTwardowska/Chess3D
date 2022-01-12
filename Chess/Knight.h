#pragma once
#include "Piece.h"
#include "Camera.h"

class Knight :public Piece {
public:
	Knight(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Knight/Knight.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {};
};