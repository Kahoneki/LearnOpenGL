#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float rotationSpeed{};

int main() {

	rotationSpeed = 1;

	int winX{800};
	int winY{800};

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	//----GENERATING VBOs AND VAOs----//
	float vertices[] = {
		// positions			// colours
		  0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	//bottom right
		 -0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	//bottom left
		  0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	//top
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	Shader ourShader("vertex.glsl", "fragment.glsl");
	

	


	//----MAIN RENDER LOOP----//
	while (!glfwWindowShouldClose(window)) {

		//inputg
		processInput(window);

		//Background colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//----RENDERING DONE HERE----//
		ourShader.use();
		
		float time = glfwGetTime() * rotationSpeed;
		
		//Transforming [-1, 1] to [-0.5, 0.5]
		float offsetXAmount = sin(time) / 2.0f;
		float offsetYAmount = cos(time) / 2.0f;

		ourShader.setFloat("xOffset", offsetXAmount);
		ourShader.setFloat("yOffset", offsetYAmount);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


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

	//Speed Adjustment//
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotationSpeed += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotationSpeed -= 0.01f;
}