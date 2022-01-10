#pragma once
#include "Model.h"
#include "Shader.h"

class Piece {
	Model model;
	Shader shader;

public:
	int x, y;
	bool isKnockedDown = false;
	glm::vec3 translateToInitialPos;
	float rotationValue = glm::radians(-90.0f);
	glm::vec3 translateVec;
	glm::vec3 scaleMatrix = glm::vec3(0.5f, 0.5f, 0.5f);
	Piece(string model, Shader shader, glm::vec3 initialPos, int x, int y) :model(Model(model)), shader(shader), translateToInitialPos(initialPos), x(x), y(y), translateVec(initialPos) {};
	void use() { shader.use(); }
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		shader.setMat4(name, mat);
	}
	void Draw() {
		if(!isKnockedDown)
			model.Draw(shader);
	}
	virtual void set(Camera camera, int width, int height) {};
	void Move(glm::vec3 moveVector) {
		translateVec = moveVector;
	}
	void KnockDown(float angle) {
		rotationValue = glm::radians(angle);
	}
};
