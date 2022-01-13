#pragma once
#include "Model.h"
#include "Shader.h"

class Board {
	Model model = Model("resources/board/board.obj");
	Shader shader = Shader("phongShader.vs", "phongShader.fs");

public :
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
	void set(Camera camera, int width, int height, glm::vec3 reflectors[], glm::vec3 reflectorDirection, bool isBlinn, float fogIntensity) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		setMat4("projection", projection);
		setMat4("view", view);
		setMat4("model", model);

		setInt("material.diffuse", 0);
		setInt("material.specular", 1);
		setFloat("material.shininess", 64.0f);

		setVec3("viewPos", camera.Position);

		setVec3("spotLight[0].position", reflectors[0]);
		setVec3("spotLight[0].direction", glm::vec3(0.0f, 0.0f, 0.0f)-reflectors[0]);
		setVec3("spotLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		setVec3("spotLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		setFloat("spotLight[0].constant", 1.0f); 
		setFloat("spotLight[0].linear", 0.003f); 
		setFloat("spotLight[0].quadratic", 0.0012f);
		setFloat("spotLight[0].cutOff", glm::cos(glm::radians(22.5f))); 
		setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(40.0f))); 

		setVec3("spotLight[1].position", reflectors[1]);
		setVec3("spotLight[1].direction", glm::vec3(0.0f, 0.0f, 0.0f) - reflectors[1]);
		setVec3("spotLight[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		setVec3("spotLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		setVec3("spotLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		setFloat("spotLight[1].constant", 1.0f);
		setFloat("spotLight[1].linear", 0.003f);
		setFloat("spotLight[1].quadratic", 0.0012f);
		setFloat("spotLight[1].cutOff", glm::cos(glm::radians(22.5f)));
		setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(40.0f)));

		setVec3("spotLight[2].position", reflectors[2]);
		setVec3("spotLight[2].direction", reflectorDirection - reflectors[2]);
		setVec3("spotLight[2].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		setVec3("spotLight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		setVec3("spotLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		setFloat("spotLight[2].constant", 1.0f);
		setFloat("spotLight[2].linear", 0.009f);
		setFloat("spotLight[2].quadratic", 0.0032f);
		setFloat("spotLight[2].cutOff", glm::cos(glm::radians(3.5f)));
		setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(4.5f)));

		setFloat("fogIntensity", fogIntensity);
		setBool("isBlinn", isBlinn);
		setBool("isBoard", true);
	}
};