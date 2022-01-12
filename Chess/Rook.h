#pragma once
#include "Piece.h"
#include "Camera.h"

class Rook :public Piece {
public:
	Rook(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Rook/Rook.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {
		differenceFromCenter = glm::vec3(-8.2f, 0.0f, 8.0f);
	};
};