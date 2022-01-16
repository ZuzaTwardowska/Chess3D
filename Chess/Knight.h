#pragma once
#include "Piece.h"
#include "Camera.h"

class Knight :public Piece {
public:
	Knight(glm::vec3 initialPos, string color, Shader s) :Piece("resources/pieces/" + color + "/Knight/Knight.obj", s, initialPos) {
		differenceFromCenter = glm::vec3(6.0f, 0.0f, -8.4f);
	};
};