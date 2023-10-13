#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <cstdlib> //For srand(), rand()

#include <iostream>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"


//------------------------------------------------//
//----FORWARD DECLARATIONS AND INITIALISATIONS----//
//------------------------------------------------//


//----FORWARD DECLARATIONS----//
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


//----SCREEN SETUP----//
constexpr int winX{800};
constexpr int winY{800};
constexpr float aspectRatio{ winX / winY };


//----CAMERA SETUP----//
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //Normalised direction vector, camera points in -z axis
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float mousePosLastFrameX = winX/2, mousePosLastFrameY = winY/2; //Initialise to center of screen
bool firstMouse = true; //Used to stop large jump in mouse movement when starting application


//----TIMING----//
float deltaTime = 0.0f; //Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame


int main() {

	//----------------------------//
	//----INITIAL OPENGL SETUP----//
	//----------------------------//

	//----INITIALISING GLFW AND GLAD, CREATING WINDOW AND VIEWPORT----//
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(winX, winY, "opengl is great", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor


	//----SET CALLBACKS----//
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	//------------------------------------------------//
	//----GENERATING VBOs, VAOs AND SHADER PROGRAM----//
	//------------------------------------------------//

	float vertices[] = {
		//Vertex Positions		//Surface Normals
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f
	};

	//----VBO----//
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//----Object VAO----//
	unsigned int objectVAO;
	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//----Light VAO----//
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Lighting shader program
	Shader lightingShader("objectVert.glsl", "objectFrag.glsl");


	//Light source shader program
	Shader lightSourceShader("lightVert.glsl", "lightFrag.glsl");
	glm::vec3 lightPos {1.2f, 1.0f, 2.0f};


	//------------------------//
	//----MAIN RENDER LOOP----//
	//------------------------//

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input
		processInput(window);

		//Clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//----SETTING UNIFORMS----//
		
		//----CAMERA MATRICES----//
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Fov), aspectRatio, 0.1f, 100.0f);


		//----OBJECT UNIFORMS----//
		lightingShader.use();
		glm::mat4 model;
		model = glm::mat4(1.0f);

		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		//Material
		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);

		//Light
		glm::vec3 lightColour;
		lightColour.x = sin(glfwGetTime() * 2.0f);
		lightColour.y = sin(glfwGetTime() * 0.7f);
		lightColour.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColour * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		lightingShader.setVec3("light.ambient", ambientColor);
		lightingShader.setVec3("light.diffuse", diffuseColor);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("lightColour", 1.0f, 1.0f, 1.0f);


		//----LIGHT SOURCE UNIFORMS----//
		lightSourceShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightSourceShader.setMat4("model", model);
		lightSourceShader.setMat4("view", view);
		lightSourceShader.setMat4("projection", projection);

		lightSourceShader.setVec3("lightColour", lightColour);


		//----RENDERING----//

		//Draw objects
		lightingShader.use();
		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw light sources
		lightSourceShader.use();
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Check & call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//De-allocate all resources
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}



//---------------------------//
//----CALLBACKS AND INPUT----//
//---------------------------//

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Movement
	const float moveSpeed = 10.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	//Wireframe toggle
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

	if (firstMouse) { //Initially true
		mousePosLastFrameX = xPos;
		mousePosLastFrameY = yPos;
		firstMouse = false;
	}

	//Refreshing variables
	float xOffset = xPos - mousePosLastFrameX;
	float yOffset = mousePosLastFrameY - yPos; //Reversed since y-coordinates range from bottom to top
	mousePosLastFrameX = xPos;
	mousePosLastFrameY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}