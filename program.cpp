#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixPercentage{0.5};

constexpr int winX{800};
constexpr int winY{800};
constexpr float aspectRatio{ winX / winY };

constexpr float PI{ 3.14159f };

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//----CREATING WINDOW AND VIEWPORT----//
	GLFWwindow* window = glfwCreateWindow(winX, winY, "opengl is pain", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window, woopsies" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD, woopsies" << std::endl;
		return -1;
	}

	glViewport(0, 0, winX, winY);
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	//----GENERATING VBOs AND VAOs----//

	float vertices[] = {
	//Positions				//Colours				//Texture Coords
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,		0.0f, 1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Texture-Coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	Shader ourShader("vertex.glsl", "fragment.glsl");
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	//----SAMPLING TEXTURE----//

	stbi_set_flip_vertically_on_load(true);

	unsigned int textures[2];
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 1" << std::endl;
	}
	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 2" << std::endl;
	}
	stbi_image_free(data);


	//----GENERATING TRANSFORMATION MATRICES----//
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection;
	projection = glm::perspective(PI / 2, aspectRatio, 0.1f, 100.0f);
	


	//----MAIN RENDER LOOP----//
	while (!glfwWindowShouldClose(window)) {

		//Input
		processInput(window);

		//Background colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		float time = glfwGetTime();

		float xOffset = sin(time) / 2;
		float yOffset = cos(time) / 2;

		ourShader.setFloat("xOffset", xOffset);
		ourShader.setFloat("yOffset", yOffset);

		float rOffset = ((sin(time) * 3) + 1) / 2;
		float gOffset = ((-cos(time) * 2) + 1) / 2;
		float bOffset = ((cos(time) * 6) + 1) / 2;

		ourShader.setFloat("rOffset", rOffset);
		ourShader.setFloat("gOffset", gOffset);
		ourShader.setFloat("bOffset", bOffset);

		glm::mat4 trans = glm::mat4(1.0f);
		float rotationAmount = sin(time) * 10;
		trans = glm::rotate(trans, rotationAmount, glm::vec3(0.0, 0.0, 1.0));

		ourShader.setMat4("rotationTransform", trans);

		ourShader.setFloat("mixPercentage", mixPercentage);

		//Rotate the cube
		model = glm::rotate(model, (float)(sin(glfwGetTime()) * 0.005), glm::vec3(0.5f, 1.0f, 0.0f));

		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);


		//----RENDERING DONE HERE----//
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//check + call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//De-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Wireframe Toggle//
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//Mix Adjustment//
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mixPercentage += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mixPercentage -= 0.01f;
}