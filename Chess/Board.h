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
};