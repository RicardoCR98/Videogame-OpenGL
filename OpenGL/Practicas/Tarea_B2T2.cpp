#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_s.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
#include <iostream>

// Es el tipo de puntero de función para las llamadas de retorno de tamaño de framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Tamaño de la ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// Declaración de variables
float visibilyFactor;
float scale = 1.0f;
float degrees;
float movementX;
float movementY;
float movementSpeed = 1.0f;
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
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: carga todos los punteros de las funciones OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Construcción del shader program
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 ourColor;

        uniform mat4 transform;

        void main()
        {
            gl_Position = transform * vec4(aPos, 1.0);
            ourColor = aColor;
        }

    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;

        uniform float visible;

        void main()
        {
            vec4 transparentColor = vec4(ourColor, visible);
            FragColor = transparentColor;
        }
    )";

    unsigned int vertexShader, fragmentShader, shaderProgram;
    int success;
    char infoLog[512];

    // Compilación del Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compilación del Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Vinculación del Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Construcción del shader program
    Shader ourShader("shaders/B2T2_VShader.vs", "shaders/B2T2_FShader.fs");

    // Establecimiento de los datos de los vértices y colores
    float vertices[] = {
        // Posiciones        // Colores RGB 
        0.0f, 0.0f, 0.0f,       1.00f, 0.78f, 0.01f,    // P1 -> indice 0 -> A -> Amarillo
        0.9f, 0.0f, 0.0f,       0.13f, 0.74f, 0.40f,    // P2 -> indice 1 -> B -> Verde oscuro
        0.35f, -0.35f, 0.0f,    0.13f, 0.74f, 0.40f,    // P3 -> indice 2 -> C -> Verde oscuro
        0.7f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,    // P4 -> indice 3 -> D -> Verde claro
        0.0f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,    // P5 -> indice 4 -> E -> verde claro
        -0.4f, -0.4f, 0.0f,     0.98f, 0.55f, 0.25f,    // P6 -> indice 5 -> F -> Naranja
        -0.1f, -0.1f, 0.0f,     0.98f, 0.55f, 0.25f,    // P7 -> indice 6 -> G -> Naranja
        -0.4f, 0.2f, 0.0f,      1.00f, 0.30f, 0.30f,    // P8 -> indice 7 -> H -> Rojo
        0.2f, 0.2f, 0.0f,       1.0f, 0.30f, 0.30f,     // P9 -> indice 8 -> I -> Rojo
        -0.1f, 0.5f, 0.0f,      0.36f, 0.44f, 0.83f,    // P10 -> indice 9 -> J -> Morado
        -0.1f, 0.6f, 0.0f,      0.36f, 0.44f, 0.83f,    // P11 -> indice 10 -> K -> Morado
        -0.6f, 0.6f, 0.0f,      0.22f, 0.69f, 0.93f,    // P12 -> indice 11 -> L -> Azul
        -0.1f, 1.0f, 0.0f,      0.22f, 0.69f, 0.93f,    // P13 -> indice 12 -> M -> Azul
        0.2f, 0.7f, 0.0f,       0.36f, 0.44f, 0.83f     // P14 -> indice 13 -> N -> Morado
    };

    unsigned int indices[] = {  // Los índices empiezan desde 0
        0,  1, 2,                // T1 -> ABC -> Primer triangulo verde oscuro
        0,  3, 4,                // T2 -> ADE -> Segundo triangulo amarillo
        0,  4, 5,                // T3 -> AEF -> Tercer triangulo verde claro
        5,  7, 6,                // T4 -> FHG -> Cuarto triangulo naranja
        7,  8, 6,                // T5 -> HIG -> Quinto Triangulo Rojo
        7, 9, 8,                 // T6 -> HJI -> Sexto Triangulo Rojo
        9, 13, 8,                // T7 -> JNI -> Septimo triangulo morado
        9, 12, 13,               // T8 -> JMN -> Octavo triangulo morado
        11, 12, 10               // T9 -> LMK -> Noveno triangulo azul
    };

    // Crear VAO, VBO y EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar los atributos de vértices
    // Posiciones
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colores RGB
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Se desvinculan VBO y VAO, pero no EBO, ya que el VAO almacena el enlace de EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Habilitar el uso de los shaders
    glUseProgram(shaderProgram);

    //################################################
     // Declaraciónes
    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/texture1.jpg", &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    // Crear una textura de OpenGL
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Configurar los parámetros de la textura con canal alfa
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar los datos de la textura a la memoria de la GPU
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }


    glGenerateMipmap(GL_TEXTURE_2D);

    // Liberar la memoria de la imagen cargada
    stbi_image_free(data);

    // Ciclo de renderizado
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // Calcular el tiempo transcurrido entre el fotograma actual y el anterior
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // Animar el protector de pantalla
        const float rotationSpeed = 80.0f;
        degrees += rotationSpeed * deltaTime;
        if (degrees > 360.0f)
            degrees -= 360.0f;

        movementX += movementSpeed * deltaTime;
        if (movementX > 1.1f || movementX < -1.1f) // Si alcanza los límites laterales
            movementSpeed = -movementSpeed; // Invertir la dirección

        movementY += movementSpeed * deltaTime;
        if (movementY > 1.1f || movementY < -1.1f) // Si alcanza los límites superiores o inferiores
            movementSpeed = -movementSpeed; // Invertir la dirección

        // Hacer que la figura se haga grande y pequeña usando una función sinusoidal
        float oscillationSpeed = 1.0f; // Velocidad de oscilación de escala
        float scaleFactor = 1.0f; // Escala máxima de oscilación
        scale = 1.0f + scaleFactor * sin(currentFrame * oscillationSpeed);

        // Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Transformaciones
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(movementX, movementY, 0.0f));
        transform = glm::rotate(transform, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale));

        // Envío de la matriz de transformación al shader
        int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


        // Activar la textura y vincularla a la unidad de textura 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        // Configurar la media opacidad
        float visibilyFactor = 0.5f; // Valor de media opacidad (0.0f a 1.0f)
        glUniform1f(glGetUniformLocation(shaderProgram, "visible"), visibilyFactor);


        // Dibujar la figura
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        // Intercambio de buffers y verificación de eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    
    }


    // Liberar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminar GLFW
    glfwTerminate();
    return 0;
}

// Función de ajuste del viewport en caso de cambio de tamaño de ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
