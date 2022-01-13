#pragma once
#include "Piece.h"
#include "Camera.h"

class Queen :public Piece {
public:
	Queen(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Queen/Queen.obj", Shader("phongShader.vs", "phongShader.fs"), initialPos, x, y) {
		differenceFromCenter = glm::vec3(1.2f, 0.0f, -8.2f);
	};
};