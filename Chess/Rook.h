#pragma once
#include "Piece.h"
#include "Camera.h"

class Rook :public Piece {
public:
	Rook(glm::vec3 initialPos, string color, Shader s) :Piece("resources/pieces/" + color + "/Rook/Rook.obj", s, initialPos) {
		differenceFromCenter = glm::vec3(-8.2f, 0.0f, 8.0f);
	};
};