//Se incluye la biblioteca glad, que se utiliza para cargar las funciones de OpenGL.
#include <glad/glad.h>
//Se incluye la biblioteca GLFW, que se utiliza para crear y administrar ventanas.
#include <GLFW/glfw3.h>
//Se incluye la biblioteca iostream para imprimir mensajes en la consola.
#include <iostream>

//Se declara una función de devolución de llamada para el cambio de tamaño del framebuffer.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Se declara una función para procesar la entrada del teclado.
void processInput(GLFWwindow* window);

// Se define el ancho y alto de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Almacenamiento del código del Vertex shader en una cadena
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Almacenamiento del código del Fragent shader en una cadena
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.9f, 0.83f, 0.16f, 1.0f);\n"
"}\n\0";


int main()
{
    //Inicializamos la libreria 
    glfwInit();
    //se configuran los ajustes del contexto de OpenGL, especificando la versión y el perfil.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Se crea una ventana de GLFW con el ancho y alto especificados en las constantes anteriores
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "1803930385 GARY RICARDO CAMPANA RAMIREZ", NULL, NULL);

    //Si la ventana no se crea correctamente, se muestra un mensaje de error y se termina el programa.
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //Se utiliza para ajustar el tamaño del framebuffer cuando cambie el tamaño de la ventana.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*Pasamos a GLAD la función para cargar la dirección de los punteros de función de OpenGL que es específica del sistema operativo.
    GLFW nos da glfwGetProcAddress que define la función correcta basada en el SO para el que estamos compilando.*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Creación de un objeto shader referenciado por un ID
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Adjunta el código fuente del shader al objeto
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //Se realiza la compilacíon del objeto shader
    glCompileShader(vertexShader);


    //Las siguientes lineas se usa para la comprobación de errores en la compilación del Vertex Shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Creación de un objeto shader referenciado por un ID
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Adjunta el código fuente del shader al objeto
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //Comprobación de errores en la compilación del Fragment Shader
    glCompileShader(fragmentShader);
    //Comprobación de errores en la compilación del Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);


    //Las siguientes lineas se usa para la comprobación de errores en la compilación del Fragment Shader
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Crea el Shader Program
    int shaderProgram = glCreateProgram();
    //Adjunta los shaders previamente compilados y los enlaza con glLinkProgra
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Se realiza la comproación de la vinculación de los shaders
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Esto de aqui es opcional, lo que hace es eliminar los objetos shaders que han sido creados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Coordenadas de los vértices
//Se añadio mas vertices
    float vertices[] = {
        0.0f,  0.5f, 0.0f, // Punto 1
        -0.433f,  0.25f,   0.0f, // Punto 2
        -0.433f, -0.25f, 0.0f, // Punto 3
       0.0f, -0.5f, 0.0f, // Punto 4
       0.433f,  -0.25f,   0.0f, // Punto 5
       0.433f,  0.25f, 0.0f  // Punto 6
    };

    //Se crea las variables VBO y VAO
    unsigned int VBO, VAO;
    //La funcion genera un objeto VAO y devuelve su identificador único, que se almacena en la variable VAO
    glGenVertexArrays(1, &VAO);
    // la funcion genera un objeto buffer y devuelve su identificador único, que se almacena en la variable VBO
    glGenBuffers(1, &VBO);
    //enlaza el VAO especificado al estado actual de OpenGL. Todas las operaciones posteriores se aplicaran al VAO enlazado
    glBindVertexArray(VAO);

    //Enlaza ek VBO al objeto GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //asigna y carga los datos del arreglo de vértices en el VBO enlazado
    // su significado es: (tipo de datos a guardar, #vertices que quiero mover, mover al VBO, se utilizaran mas veces)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Establacimiento de los punteros de los atributos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //se utiliza para desenlazar el VBO actualmente enlazado, estableciendole en 0 o null
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //se utiliza para desenlazar el VAO actualmente enlazado, estableciendole en 0 o null
    glBindVertexArray(0);


    // render loop
    //  bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        //Color de fondo
        glClearColor(0.21f, 0.16f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Dibujo del triángulo
        //Uso de la aplicación
        glUseProgram(shaderProgram);
        //Uso de datos
        glBindVertexArray(VAO);
        //Tipo de primitiva, indice de iniciacio, vertices a dibujar
//Se cambio la variable GL_TRIANGLE por GL_TRIANGLE_FAN y en vez de 3 vertices, se puso 6
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

        //Intercambia los buffers delantero y trasero de la ventana especificada
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Eliminar los objetos que fueron enlazados
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //Termina con la librería GLFW
    glfwTerminate();
    return 0;
}

//Si se seleciona la tecla esc, se cierra la ventana
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//Las coordenadas normalizadas se transformarán en coordenadas de espacio de pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}