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
	void setVec3(const std::string& name, const glm::vec3& mat) const {
		shader.setVec3(name, mat);
	}
	void setInt(const std::string& name, int value) const
	{
		shader.setInt(name, value);
	}
	void setFloat(const std::string& name, float value) const
	{
		shader.setFloat(name, value);
	}
	void Draw() {
		if(!isKnockedDown)
			model.Draw(shader);
	}
	void set(Camera camera, int width, int height, glm::vec3 mainReflector) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, translateVec);
		model = glm::rotate(model, rotationValue, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, scaleMatrix);
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);

		setInt("material.diffuse", 0);
		setInt("material.specular", 1);
		setFloat("material.shininess", 64.0f);

		setVec3("viewPos", camera.Position);

		setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		setVec3("light.position", mainReflector);
	}
	void Move(glm::vec3 moveVector) {
		translateVec = moveVector;
	}
	void KnockDown(float angle) {
		rotationValue = glm::radians(angle);
	}
};
