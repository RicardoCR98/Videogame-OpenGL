#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#define STB_IMAGE_IMPLEMENTATION  
#include "learnopengl/stb_image.h"  
#include <iostream>

/*Es el tipo de puntero de funci�n para las llamadas de retorno de tama�o de framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/*Tama�o de la ventana*/
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

float visibilyFactor;

int main()
{
	/* Inicializa la libreria GLFW*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Crear una ventana y su contexto OpenGL */
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gary Campana, 1803930385", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: carga todos los punteros de las funciones OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Construir los shader program
	Shader ShaderProgram1("shaders/B2T1_VShader.vs", "shaders/B2T1_FShader.fs");
	Shader ShaderProgram2("shaders/B2T1_VShader.vs", "shaders/B2T1_FShader.fs");

	// Establecimiento de los datos de los v�rtices
	float vertices[] = {
		// Posiciones				// Colores RGB			//Texturas
		0.0f, 0.0f, 0.0f,       1.00f, 0.78f, 0.01f,		0.3f, 1.0f,     // P1 -> indice 0 -> A -> Amarillo
		0.9f, 0.0f, 0.0f,       0.13f, 0.74f, 0.40f,		1.0f, 1.0f,     // P2 -> indice 1 -> B -> Verde oscuro
		0.35f, -0.35f, 0.0f,    0.13f, 0.74f, 0.40f,		0.5f, 0.6f,		// P3 -> indice 2 -> C -> Verde oscuro
		0.7f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,		0.75f, 0.0f,	// P4 -> indice 3 -> D -> Verde claro
		0.0f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,		0.3f, 0.0f,		// P5 -> indice 4 -> E -> verde claro
		-0.4f, -0.4f, 0.0f,     0.98f, 0.55f, 0.25f,		0.0f, 0.0f,		// P6 -> indice 5 -> F -> Naranja


		-0.1f, -0.1f, 0.0f,     0.98f, 0.55f, 0.25f,		0.5f, 0.2f,		// P7 -> indice 6 -> G -> Naranja
		-0.4f, 0.2f, 0.0f,      1.00f, 0.30f, 0.30f,		0.2f, 0.4f,		// P8 -> indice 7 -> H -> Rojo
		0.2f, 0.2f, 0.0f,       1.0f, 0.30f, 0.30f,			1.0f, 0.4f,     // P9 -> indice 8 -> I -> Rojo
		-0.1f, 0.5f, 0.0f,      0.36f, 0.44f, 0.83f,		0.5f, 0.6f,     // P10 -> indice 9 -> J -> Morado
		-0.1f, 0.6f, 0.0f,      0.36f, 0.44f, 0.83f,		0.5f, 0.7f,		// P11 -> indice10 -> K -> Morado
		-0.6f, 0.6f, 0.0f,      0.22f, 0.69f, 0.93f,		0.0f, 0.7f,		// P12 -> indice11 -> L -> Azul
		-0.1f, 1.0f, 0.0f,      0.22f, 0.69f, 0.93f,		0.5f, 1.0f,		// P13 -> indice12 -> M -> Azul
		0.2f, 0.7f, 0.0f,       0.36f, 0.44f, 0.83f,		1.0f, 0.75f,     // P14 -> indice13 -> N -> Morado

	};

	unsigned int indicesGrupo1[] = {  // Los �ndices empiezan desde 0
		 0,  1, 2,                // T1 -> ABC -> Primer triangulo verde oscuro
		 0,  3, 4,                // T2 -> ADE -> Segundo triangulo amarillo
		 0,  4, 5,                // T3 -> AEF -> Tercer triangulo verde claro
	};

	unsigned int indicesGrupo2[] = {  // Los �ndices empiezan desde 0
		5,  7, 6,                // T4 -> FHG -> Cuarto triangulo naranja
		7,  8, 6,                // T5 -> HIG -> Quinto Triangulo Rojo
		7, 9, 8,                 // T6 -> HJI -> Sexto Triangulo Rojo
		9, 13, 8,                // T7 -> JNI -> Septimo triangulo morado
		9, 12, 13,               // T8 -> JMN -> Octavo triangulo morado
		11, 12,10                // T9 -> LMK -> Noveno triangulo azul
	};

	//###########################################################################################
	// Declaraciones
	unsigned int VBO[2], VAO[2], EBO[2], textures[2];
	glGenVertexArrays(2, VAO);
	// Generaci�n de buffers
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);
	glGenTextures(2, textures);

	//01. Configuraci�n del primer grupo
	//Uso del Vertex Array Object
	glBindVertexArray(VAO[0]);
	//Enlaza el objeto de matriz de v�rtices y establece el b�fer de v�rtices
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Enlaza el objeto de matriz de elementos y establece el b�fer de elementos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGrupo1), indicesGrupo1, GL_STATIC_DRAW);
	//Establacimiento de los punteros de los atributos de los v�rtices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Establacimiento de los punteros de los atributos de los colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Establecimiento de los punteros de las coordenadas de textura 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//02. Configuraci�n del segundo grupo
	//Uso del Vertex Array Object
	glBindVertexArray(VAO[1]);
	//Enlaza el objeto de matriz de v�rtices y establece el b�fer de v�rtices
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Enlaza el objeto de matriz de elementos y establece el b�fer de elementos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGrupo2), indicesGrupo2, GL_STATIC_DRAW);
	//Establacimiento de los punteros de los atributos de los v�rtices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Establacimiento de los punteros de los atributos de los colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Establecimiento de los punteros de las coordenadas de textura 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//###################################################################################################
	// Carga y creaci�n de la textura 
	// Textura 1
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// Establece los par�metros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Establece los par�metros de filtrado 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Carga la imagen, crea la textura y genera el mipmap 
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Cambia el modo de lectura del eje
 	unsigned char* data = stbi_load("textures/textura2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{   
		//Copia la textura desde la CPU(data) hacia la GPU(texture)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Textura 2
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// Establece los par�metros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Establece los par�metros de filtrado 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga la imagen, crea la textura y genera el mipmap 
	data = stbi_load("textures/textura1.jpg", &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true); // Cambia el modo de lectura del eje
	if (data)
	{
		//Copia la textura desde la CPU(data) hacia la GPU(texture)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//###########################################################################################
	// Indica a Opengl a qu� unidad de textura pertenece
	ShaderProgram1.use(); 
	ShaderProgram1.setInt("textura", 0);

	ShaderProgram2.use(); 
	ShaderProgram2.setInt("textura", 1);
	
	//###########################################################################################
	// Bucle de renderizado
	while (!glfwWindowShouldClose(window))
	{
		// Entrada
		processInput(window);
		// Renderizado 
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Color de fondo
		glClear(GL_COLOR_BUFFER_BIT);

		// Carga de la textura 1 en el shaderProgram1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		// Configuraci�n del factor de visibilidad de la textura 
		ShaderProgram1.setFloat("visible", visibilyFactor);
		// Dibujar los tri�ngulos Grupo1   
		ShaderProgram1.use();
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


		// Carga de la textura 2 en el shaderProgram2
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		// Configuraci�n del factor de visibilidad de la textura 
		ShaderProgram2.setFloat("visible", visibilyFactor);
		// Dibujar los tri�ngulos Grupo2
		ShaderProgram2.use();
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		// Configuraci�n del rango de cambio de tiempo
		float timeValue = glfwGetTime();
		visibilyFactor = sin(timeValue) / 2.0f + 0.5f;

		// Intercambia los buffers delantero y trasero de la ventana especificada
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*Opcional: Eliminar los objetos que fueron enlazados*/
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	glDeleteBuffers(1, EBO);

	//glfw: termina borrando todos los recursos GLFW previamente asignados.glfwTerminate();
	glfwTerminate();
	return 0;
}

// Si se han pulsado / soltado las teclas pertinentes --> reaccionar en consecuencia
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //Si se seleciona la tecla esc, se cierra la ventana
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)       //Si se seleciona la tecla 1, muestra las primitivas con sus respectivos colores
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)       //Si se seleciona la tecla 2, muestra solo las l�neas
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

//Las coordenadas normalizadas se transformar�n en coordenadas de espacio de pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}