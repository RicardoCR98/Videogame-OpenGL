#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Para redimensionar la figura dentro de la ventan
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Para realizar operaciones relacionadas con la entrada de usuario
void processInput(GLFWwindow* window);

//Variables constantes
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
	//Si no se inicia glf entra
	if (!glfwInit()) {
		std::cout << "Fallo al iniciar GLFW" << std::endl;
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1", NULL, NULL);

	if (!window)
	{
		// Ocurrió un error al crear la ventana
		std::cout << "No se pudo crear la ventana" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Hacer que la ventana sea el contexto actual
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// glad: cargar todos los punteros de función OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fallo al iniciar GLAD" << std::endl;
		return -1;
	}

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//Lee algún input del usuario
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Procesar eventos
		glfwPollEvents();
		// Intercambiar los búferes de la ventana (para evitar el parpadeo)
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//establece la transformación de coordenadas
	//x, y, ancho, alto
	glViewport(0, 0, width, height);
}