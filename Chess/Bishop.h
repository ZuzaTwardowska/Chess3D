#pragma once
#include "Piece.h"
#include "Camera.h"

class Bishop :public Piece {
public:
	Bishop(glm::vec3 initialPos, string color, Shader s) :Piece("resources/pieces/" + color + "/Bishop/Bishop.obj", s, initialPos) {
		differenceFromCenter = glm::vec3(3.8f, 0.0f, -8.4f);
	};
};