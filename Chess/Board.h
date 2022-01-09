#pragma once
#include "Model.h"
#include "Shader.h"

class Board {
	Model model = Model("resources/board/board.obj");
	Shader shader = Shader("boardShader.vs", "boardShader.fs");

public :
	void use() { shader.use(); }
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		shader.setMat4(name, mat);
	}
	void Draw() {
		model.Draw(shader);
	}
	void set(Camera camera, int width, int height) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);
	}
};