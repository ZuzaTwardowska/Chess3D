#pragma once
#include "Piece.h"
#include "Camera.h"

class Pawn :public Piece {
public:
	Pawn(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/Pawn/Pawn.obj", Shader("phongShader.vs", "phongShader.fs"), initialPos, x, y) {
		differenceFromCenter = glm::vec3(8.3f, 0.0f, -5.9f);
	};
};