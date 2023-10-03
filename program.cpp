#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//vertex shader
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSourceOrange =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSourceYellow =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int GenerateShaderProgram(int colourCode);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//----CREATING WINDOW AND VIEWPORT----//
	GLFWwindow* window = glfwCreateWindow(800, 800, "opengl is pain", NULL, NULL);
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

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	//----GENERATING VBOs AND VAOs----//
	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};
	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//Generating first triangle//
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	//Vertex attributes stay the same
	glEnableVertexAttribArray(0);

	//Generating second triangle//
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	//Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	unsigned int shaderProgramOrange = GenerateShaderProgram(0);
	unsigned int shaderProgramYellow = GenerateShaderProgram(1);


	//----MAIN RENDER LOOP----//
	while (!glfwWindowShouldClose(window)) {

		//input
		processInput(window);

		//Background colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//----RENDERING DONE HERE----//
		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		//check + call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//De-allocate all resources
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);

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
}


int GenerateShaderProgram(int colourCode) { //0=orange, 1=yellow
	//Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Making sure vertex shader compiled properly.
	{
		int successVert;
		char infoLogVert[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
		if (!successVert) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVert);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogVert << std::endl;
		}
	}

	//Fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (colourCode == 0)
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceOrange, NULL);
	else if (colourCode == 1)
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShader);

	//Making sure fragment shader is compiled properly.
	{
		int successFrag;
		char infoLogFrag[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
		if (!successFrag) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFrag);
			std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLogFrag << std::endl;
		}
	}

	//Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Making sure shader program is linked correctly
	{
		int successShader;
		char infoLogShader[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successShader);
		if (!successShader) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogShader);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLogShader << std::endl;
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}