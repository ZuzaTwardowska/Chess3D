#pragma once
#include "Piece.h"
#include "Camera.h"

class Knight :public Piece {
public:
	Knight(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Knight/Knight.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {
		differenceFromCenter = glm::vec3(6.0f, 0.0f, -8.4f);
	};
};