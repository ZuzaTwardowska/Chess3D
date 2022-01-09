#pragma once
#include "Piece.h"
#include "Camera.h"

class King :public Piece {
public:
	King(glm::vec3 initialPos, string color, int x, int y) :Piece("resources/pieces/" + color + "/King/King.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos, x, y) {};
	void set(Camera camera, int width, int height) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, translateVec);
		model = glm::rotate(model, rotationValue, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, scaleMatrix);
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);
	}
};