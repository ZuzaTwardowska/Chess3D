#pragma once
#include "Piece.h"
#include "Camera.h"

class Pawn :public Piece {
public:
	Pawn(glm::vec3 initialPos, string color) :Piece("resources/pieces/"+color+"/Pawn/Pawn.obj", Shader("pieceShader.vs", "pieceShader.fs"), initialPos) {};
	void set(Camera camera, int width, int height) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, translateToInitialPos); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, scaleMatrix);	// it's a bit too big for our scene, so scale it down
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);
	}
};