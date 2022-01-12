#pragma once
#include "Piece.h"
#include "Camera.h"

class King :public Piece {
public:
	King(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/King/King.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {};
};