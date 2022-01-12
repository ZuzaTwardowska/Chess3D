#pragma once
#include "Piece.h"
#include "Camera.h"

class Bishop :public Piece {
public:
	Bishop(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Bishop/Bishop.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {
		differenceFromCenter = glm::vec3(3.8f, 0.0f, -8.4f);
	};
};