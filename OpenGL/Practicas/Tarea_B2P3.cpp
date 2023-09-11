#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
#include <iostream>

// Funciones para el buffer, mouse, scroll y procesamiento de entradas
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Tamaño de la ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// Configuración de la cámara
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f)); // Posición de la cámara
float lastX = SCR_WIDTH / 2.0f;             // Posición del mouse respecto al eje X  
float lastY = SCR_HEIGHT / 2.0f;            // Posición del mouse respecto al eje Y

// Definiendo una variable booleana global para comprobar si este es la primera vez que se recibe una entrada del mouse
bool firstMouse = true;

// Cronometraje
float deltaTime = 0.0f;	// Tiempo entre la trama actual y la última trama
float lastFrame = 0.0f; // Tiempo de la última trama 

int main()
{
	// Inicializa la libreria GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Crear una ventana y su contexto OpenGL 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gary Campana 1803930385", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Hace que el contexto de la ventana especificada sea actual para el hilo que llama
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Es el llamado cuando el framebuffer de la ventana especificada.
	glfwSetCursorPosCallback(window, mouse_callback); // Devolución de llamada cada vez que se mueve el mouse
	glfwSetScrollCallback(window, scroll_callback); // Devolución de llamada de desplazamiento (uso del scroll)

	// Capturar un cursor (el cursor del mouse permanece en el centro de la ventana)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: carga todos los punteros de las funciones OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Se habilitan las pruebas de profundidad (permite a OpenGL decidircuándo dibujar sobre un píxel y cuándo no)
	glEnable(GL_DEPTH_TEST);

	//##########################	CARGA DE SHADERS	#############################
	Shader ourShader("shaders/B2T3_VShader.vs", "shaders/B2T3_FShader.fs");



	//##########################	VERTICES 	#############################
	// Establecimiento de los datos de los vértices
	float vertices[] = {
	//CUBO DE 842 X 595
	// Posiciones             //Coordenadas de la textura
	 // Cara lateral trasera
	-0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,
	 0.5f, -0.5f, -0.5f,/*P1*/     0.543f,0.492f,
	 0.5f,  0.5f, -0.5f,/*P2*/     0.714f,0.492f,
	 0.5f,  0.5f, -0.5f,/*P2*/     0.714f,0.492f,
	-0.5f,  0.5f, -0.5f,/*P3*/     0.714f,0.328f,
	-0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,

	// Cara delantera
   -0.5f, -0.5f,  0.5f,/*P4*/     0.373f, 0.328f,
	0.5f, -0.5f,  0.5f,/*P5*/     0.373f, 0.492f,
	0.5f,  0.5f,  0.5f,/*P6*/     0.202f, 0.492f,
	0.5f,  0.5f,  0.5f,/*P6*/     0.202f, 0.492f,
   -0.5f,  0.5f,  0.5f,/*P7*/     0.202f, 0.328f,
   -0.5f, -0.5f,  0.5f,/*P4*/     0.373f, 0.328f,

   //Cara lateral izquierda
   -0.5f,  0.5f,  0.5f,/*P7*/     0.714f, 0.084f,
   -0.5f,  0.5f, -0.5f,/*P3*/     0.714f,0.328f,
   -0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,
   -0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,
   -0.5f, -0.5f,  0.5f,/*P4*/     0.543f, 0.084f,
   -0.5f,  0.5f,  0.5f,/*P7*/     0.714f, 0.084f,

   // Cara lateral derecha
	0.5f,  0.5f,  0.5f,/*P6*/     0.714f, 0.734f,
	0.5f,  0.5f, -0.5f,/*P2*/     0.714f,0.492f,
	0.5f, -0.5f, -0.5f,/*P1*/     0.543f,0.492f,
	0.5f, -0.5f, -0.5f,/*P1*/     0.543f,0.492f,
	0.5f, -0.5f,  0.5f,/*P5*/     0.543f, 0.734f,
	0.5f,  0.5f,  0.5f,/*P6*/     0.714f, 0.734f,

	// Cara inferior
   -0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,
	0.5f, -0.5f, -0.5f,/*P1*/     0.543f,0.492f,
	0.5f, -0.5f,  0.5f,/*P5*/     0.373f, 0.492f,
	0.5f, -0.5f,  0.5f,/*P5*/     0.373f, 0.492f,
   -0.5f, -0.5f,  0.5f,/*P4*/     0.202f, 0.492f,
   -0.5f, -0.5f, -0.5f,/*P0*/     0.543f,0.328f,

   // Cara superior
  -0.5f,  0.5f, -0.5f,/*P3*/     0.714f,0.328f,
   0.5f,  0.5f, -0.5f,/*P2*/     0.714f,0.492f,
   0.5f,  0.5f,  0.5f,/*P6*/     0.885f, 0.492f,
   0.5f,  0.5f,  0.5f,/*P6*/     0.885f, 0.492f,
  -0.5f,  0.5f,  0.5f,/*P7*/     0.885f, 0.328f,
  -0.5f,  0.5f, -0.5f,/*P3*/     0.714f,0.328f,
	
 // Cubo con coordendas 600 x 500
  // Cara lateral trasera
	  -0.5f, -0.5f, -0.5f,/*P0*/    0.500f, 0.418f,
	   0.5f, -0.5f, -0.5f,/*P1*/    0.500f, 0.666f,
	   0.5f,  0.5f, -0.5f,/*P2*/    0.723f, 0.666f,
	   0.5f,  0.5f, -0.5f,/*P2*/    0.723f, 0.666f,
	  -0.5f,  0.5f, -0.5f,/*P3*/    0.723f, 0.418f,
	  -0.5f, -0.5f, -0.5f,/*P0*/    0.500f, 0.418f,

	  // Cara delantera
	 -0.5f, -0.5f,  0.5f,/*P4*/     0.295f, 0.418f,
	  0.5f, -0.5f,  0.5f,/*P5*/     0.295f,	0.666f,
	  0.5f,  0.5f,  0.5f,/*P6*/     0.083f,	0.666f,
	  0.5f,  0.5f,  0.5f,/*P6*/     0.083f, 0.666f,
	 -0.5f,  0.5f,  0.5f,/*P7*/     0.083f,	0.418f,
	 -0.5f, -0.5f,  0.5f,/*P4*/     0.295f, 0.418f,

	 //Cara lateral izquierda
	 -0.5f,  0.5f,  0.5f,/*P7*/     0.295f,	0.164f,
	 -0.5f,  0.5f, -0.5f,/*P3*/     0.500f,	0.164f,
	 -0.5f, -0.5f, -0.5f,/*P0*/     0.500f,	0.418f,
	 -0.5f, -0.5f, -0.5f,/*P0*/     0.500f, 0.418f,
	 -0.5f, -0.5f,  0.5f,/*P4*/     0.295f,	0.418f,
	 -0.5f,  0.5f,  0.5f,/*P7*/     0.295f, 0.164f,

	 // Cara lateral derecha
	  0.5f,  0.5f,  0.5f,/*P6*/     0.295f,	0.920f,
	  0.5f,  0.5f, -0.5f,/*P2*/     0.500f,	0.920f,
	  0.5f, -0.5f, -0.5f,/*P1*/     0.500f,	0.666f,
	  0.5f, -0.5f, -0.5f,/*P1*/     0.500f, 0.666f,
	  0.5f, -0.5f,  0.5f,/*P5*/     0.295f,	0.666f,
	  0.5f,  0.5f,  0.5f,/*P6*/     0.295f, 0.920f,

	  // Cara inferior
	 -0.5f, -0.5f, -0.5f,/*P0*/     0.500f,	0.418f,
	  0.5f, -0.5f, -0.5f,/*P1*/     0.500f,	0.666f,
	  0.5f, -0.5f,  0.5f,/*P5*/     0.295f,	0.666f,
	  0.5f, -0.5f,  0.5f,/*P5*/     0.295f, 0.666f,
	 -0.5f, -0.5f,  0.5f,/*P4*/     0.295f,	0.418f,
	 -0.5f, -0.5f, -0.5f,/*P0*/     0.500f, 0.418f,

	 // Cara superior
	-0.5f,  0.5f, -0.5f,/*P3*/      0.723f,	0.418f,
	 0.5f,  0.5f, -0.5f,/*P2*/      0.723f,	0.666f,
	 0.5f,  0.5f,  0.5f,/*P6*/      0.933f,	0.666f,
	 0.5f,  0.5f,  0.5f,/*P6*/      0.933f, 0.666f,
	-0.5f,  0.5f,  0.5f,/*P7*/      0.933f,	0.418f,
	-0.5f,  0.5f, -0.5f,/*P3*/      0.723f, 0.418f

	};


	// Pocisiones del cubo en el world space
	glm::vec3 cubePositions[] = {
		glm::vec3(1.5f,  1.5f, -1.5f),	//Cara
		glm::vec3(0.0f,  0.0f, -1.5f),	//Cara
		glm::vec3(-1.5f, -1.5f, -1.5f),	//Cara
		glm::vec3(1.5f,  1.5f, -5.5f),	//Fuego
		glm::vec3(0.0f,  0.0f, -5.5f),	//Fuego
		glm::vec3(-1.5f, -1.5f, -5.5f),	//Fuego
		glm::vec3(1.5f,  1.5f, -9.5f),	//Tierra
		glm::vec3(0.0f,  0.0f, -9.5f),	//Tierra
		glm::vec3(-1.5f,  -1.5f, -9.5f)	//Tierra
	};

	//##########################	VAO VBO 	#############################
	// Declaraciones VBO,VAO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	// Generación de buffers
	glGenBuffers(1, &VBO);
	// Uso del Vertex Array Object
	glBindVertexArray(VAO);
	// Enlaza el objeto de matriz de vértices y establece el búfer de vértices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Establacimiento de los punteros de los atributos de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Establecimiento de los punteros de las coordenadas de textura 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

//##########################	TEXTURAS	#############################
	// Carga y creación de las texturas
	// -------------------------
	unsigned int texture1, texture2, texture3; // Declaración de texturas 
	// Textura 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Establece los parámetros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Establece los parámetros de filtrado 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga la imagen, crea la textura y genera el mipmap 
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false); // Cambia el modo de lectura del eje (cuando está en true).
	unsigned char* data = stbi_load("textures/Texture4.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//Copia la textura desde la CPU(data) hacia la GPU(texture)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Textura 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Establece los parámetros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Establece los parámetros de filtrado 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga la imagen, crea la textura y genera el mipmap 
	data = stbi_load("textures/texture5.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//Copia la textura desde la CPU(data) hacia la GPU(texture)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// Textura 3
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// Establece los parámetros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Establece los parámetros de filtrado 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga la imagen, crea la textura y genera el mipmap 
	data = stbi_load("textures/Texture6.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//Copia la textura desde la CPU(data) hacia la GPU(texture)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Indica a Opengl a qué unidad de textura pertenece
	ourShader.use();
	ourShader.setInt("texture1", 0);


	//##########################	RENDER LOOP 	#############################

	while (!glfwWindowShouldClose(window))
	{
		// Lógica de tiempo por frame (calculo del nuevo valor deltaTime para uso posterior)
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Entrada
		processInput(window);

		// Renderizado 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Color de fondo

		// Borrado del búfer de color y el búfer de profundidad 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enlaza las texturas en las unidades de textura correspondientes
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Uso del Shader Program
		ourShader.use();

		// Establecer la matriz de proyección (pasar la matriz de proyección al shader)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection); // Nota: actualmente establecemos la matriz de proyección cada frame, 

		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		// Renderización de las cajas
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 9; i++)
		{

		// Calcula la matriz del modelo para cada objeto y pasarla al shader antes de dibujar
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 30.0f * i;

			if (i < 3) // A los primeros 3 bloques añadimos movimiento  - cara - 
			{
				angle = glfwGetTime() * 200.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				glBindTexture(GL_TEXTURE_2D, texture1);
			}
			else if (i >= 3 && i < 6) // A los bloques 4, 5 y 6 se les hace pequeños y grandes - fuego -
			{
				float scaleFactor = sin(glfwGetTime() * 5.0f) * 0.5f + 1.0f;
				model = glm::scale(model, glm::vec3(scaleFactor));
				glBindTexture(GL_TEXTURE_2D, texture2);
			}
			else // A los bloques restantes se les añade rotación -tierra- 
			{
				angle = glfwGetTime() * 200.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(-1.5f, 1.5f, 0.0f));
				glBindTexture(GL_TEXTURE_2D, texture3);

			}
			ourShader.setMat4("model", model);
			

		
			if (i < 3)
			{
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 36, 72);
			}

		}
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Opcional: Eliminar los objetos que fueron enlazados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: termina borrando todos los recursos GLFW previamente asignados.glfwTerminate();
	glfwTerminate();
	return 0;
}

// Si se han pulsado / soltado las teclas pertinentes --> reaccionar en consecuencia
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)   //Si se seleciona la tecla esc, se cierra la ventana
		glfwSetWindowShouldClose(window, true);

	// La posición de la cámara se actualiza en consecuencia
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)        //Si se seleciona la tecla W, se acerca la cámara
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)        //Si se seleciona la tecla S, se aleja la camara
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)        //Si se seleciona la tecla A, se desplaza la camara hacia la izquierda
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)        //Si se seleciona la tecla D, se desplaza la camara hacia la derecha
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

//Las coordenadas normalizadas se transformarán en coordenadas de espacio de pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: cada vez que el ratón se mueve, se llama a este callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // Actualizamos las posiciones iniciales del mouse a los nuevos valores xpos y ypos
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX; // Desplazamiento del scroll en X
	float yoffset = lastY - ypos; // Desplazamiento del scroll en Y (Invertido, puesto que las coordenadas sondiferentes)
	lastX = xpos; // Última posición en X
	lastY = ypos; // Última posición en Y

	camera.ProcessMouseMovement(xoffset, yoffset); // Procesamiento del movimiento del scroll
}

// glfw: cada vez que la rueda del ratón se desplaza, se llama a esta función
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}