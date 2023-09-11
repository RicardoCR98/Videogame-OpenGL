#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>

/*Es el tipo de puntero de función para las llamadas de retorno de tamaño de framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/*Dimension de la ventana*/
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    /* Inicializa la libreria GLFW*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Crear una ventana y su contexto OpenGL */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1P3 Gary Campana", NULL, NULL);
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

    // Construir y compilar el shader program
    Shader ourShader("shaders/B1P3_VShader.vs", "shaders/B1P3_FShader.fs");

    // Establecimiento de los datos de los vértices
    float vertices[] = {
        // Posiciones        // Colores RGB 
        0.0f, 0.0f, 0.0f,       1.00f, 0.78f, 0.01f,    // P1 -> indice 0 -> A -> Amarillo
        0.9f, 0.0f, 0.0f,       0.13f, 0.74f, 0.40f,    // P2 -> indice 1 -> B -> Verde oscuro
        0.35f, -0.35f, 0.0f,    0.13f, 0.74f, 0.40f, // P3 -> indice 2 -> C -> Verde oscuro
        0.7f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,   // P4 -> indice 3 -> D -> Verde claro
        0.0f, -0.7f, 0.0f,      0.56f, 0.81f, 0.20f,   // P5 -> indice 4 -> E -> verde claro
        -0.4f, -0.4f, 0.0f,     0.98f, 0.55f, 0.25f,  // P6 -> indice 5 -> F -> Naranja
        -0.1f, -0.1f, 0.0f,     0.98f, 0.55f, 0.25f,  // P7 -> indice 6 -> G -> Naranja
        -0.4f, 0.2f, 0.0f,      1.00f, 0.30f, 0.30f,   // P8 -> indice 7 -> H -> Rojo
        0.2f, 0.2f, 0.0f,       1.0f, 0.30f, 0.30f,     // P9 -> indice 8 -> I -> Rojo
        -0.1f, 0.5f, 0.0f,      0.36f, 0.44f, 0.83f,   // P10 -> indice 9 -> J -> Morado
        -0.1f, 0.6f, 0.0f,      0.36f, 0.44f, 0.83f,   // P11 -> indice10 -> K -> Morado
        -0.6f, 0.6f, 0.0f,      0.22f, 0.69f, 0.93f,   // P12 -> indice11 -> L -> Azul
        -0.1f, 1.0f, 0.0f,      0.22f, 0.69f, 0.93f,   // P13 -> indice12 -> M -> Azul
        0.2f, 0.7f, 0.0f,       0.36f, 0.44f, 0.83f,    // P14 -> indice13 -> N -> Morado

    };
    
    unsigned int indices[] = {  // Los índices empiezan desde 0
     0,  1, 2,                // T1 -> ABC -> Primer triangulo verde oscuro

      0,  3, 4,                // T2 -> ADE -> Segundo triangulo amarillo

      0,  4, 5,                 // T3 -> AEF -> Tercer triangulo verde claro

      5,  7, 6,                 // T4 -> FHG -> Cuarto triangulo naranja

      7,  8, 6,                 // T5 -> HIG -> Quinto Triangulo Rojo
      7, 9, 8,                 // T6 -> HJI -> Sexto Triangulo Rojo

      9, 13, 8,                 // T7 -> JNI -> Septimo triangulo morado
      9, 12, 13,                 // T8 -> JMN -> Octavo triangulo morado

      11, 12,10                 // T9 -> LMK -> Noveno triangulo azul
    };

    //Declaraciones
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    //Generación de buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //Uso del Vertex Array Object
    glBindVertexArray(VAO);
    //Enlaza el objeto de matriz de vértices y establece el búfer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Enlaza el objeto de matriz de elementos y establece el búfer de elementos
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Establacimiento de los punteros de los atributos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Establacimiento de los punteros de los atributos de los colores
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Entrada
        processInput(window);

        // Renderizado 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        //  Dibujar los triángulos
        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);

        // Intercambia los buffers delantero y trasero de la ventana especificada
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*Opcional: Eliminar los objetos que fueron enlazados*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)       //Si se seleciona la tecla 2, muestra solo las líneas
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
}

    //Las coordenadas normalizadas se transformarán en coordenadas de espacio de pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}