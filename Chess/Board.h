#pragma once
#include "Model.h"
#include "Shader.h"

class Board {
	Model model = Model("resources/board/board.obj");
	Shader shader;

public :
	Board(Shader s) : shader(s) {};
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
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);

		setVec3("viewPos", camera.Position);

		setVec3("spotLight[0].position", reflectors[0]);
		setVec3("spotLight[0].direction", glm::vec3(0.0f, 0.0f, 0.0f)-reflectors[0]);
		setVec3("spotLight[0].ambient", glm::vec3(lightIntensity));

		setVec3("spotLight[1].position", reflectors[1]);
		setVec3("spotLight[1].direction", glm::vec3(0.0f, 0.0f, 0.0f) - reflectors[1]);
		setVec3("spotLight[1].ambient", glm::vec3(lightIntensity));

		setVec3("spotLight[2].position", reflectors[2]);
		setVec3("spotLight[2].direction", reflectorDirection - reflectors[2]);

		setFloat("fogIntensity", fogIntensity);
		setBool("isBlinn", isBlinn);
	}
	void setShader(Shader s) {
		shader = s;
	}
};