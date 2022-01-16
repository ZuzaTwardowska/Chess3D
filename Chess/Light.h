#pragma once
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include <cmath>

class Light {
	Model model = Model("resources/light/light.obj");
	Shader shader;

public:
	glm::vec3 position;
	float yrotation;
	float xrotation = 0;
	Light(glm::vec3 pos, float rot, Shader s) :position(pos), yrotation(rot), shader(s) {};
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
	void setBool(const std::string& name, bool value) const
	{
		shader.setBool(name, value);
	}
	void Draw() {
		model.Draw(shader);
	}

	void set(Camera camera, int width, int height, glm::vec3 reflectors[], glm::vec3 reflectorDirection, bool isBlinn, float fogIntensity, float lightIntensity) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, yrotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, xrotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);

		setVec3("viewPos", camera.Position);

		setVec3("spotLight[0].position", reflectors[0]);
		setVec3("spotLight[0].direction", glm::vec3(0.0f, 0.0f, 0.0f) - reflectors[0]);
		setVec3("spotLight[0].ambient", glm::vec3(lightIntensity));

		setVec3("spotLight[1].position", reflectors[1]);
		setVec3("spotLight[1].direction", glm::vec3(0.0f, 0.0f, 0.0f) - reflectors[1]);
		setVec3("spotLight[1].ambient", glm::vec3(lightIntensity));


		setVec3("spotLight[2].position", reflectors[2]);
		setVec3("spotLight[2].direction", reflectorDirection - reflectors[2]);

		setFloat("fogIntensity", fogIntensity);
		setBool("isBlinn", isBlinn);

		setBool("isBoard", false);
	}
	void setShader(Shader s) {
		shader = s;
	}
	void changeDirection(glm::vec3 direction) {
		yrotation = glm::radians(-90.0f) - atan((-direction[0] + position[0]) / (-direction[1] + position[1]));
		xrotation = atan((-direction[2] + position[2]) / (-direction[1] + position[1]));
	}
};