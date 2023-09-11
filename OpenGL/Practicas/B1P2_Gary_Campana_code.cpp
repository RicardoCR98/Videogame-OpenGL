#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
/*Es el tipo de puntero de función para las llamadas de retorno de tamaño de framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
/*Tamaño de la ventana*/
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*Almacenamiento del código del Vertex shader en una cadena*/
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
/*Almacenamiento del código del Fragent shader en una cadena*/

/*Verde oscuro*/
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.13f, 0.74f, 0.40f, 1.0f);\n"
"}\n\0";

/*Color amarillo*/
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00f, 0.78f, 0.01f, 1.0f);\n"
"}\n\0";

/*Color verde claro*/
const char* fragmentShader3Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.56f, 0.81f, 0.20f, 1.0f);\n"
"}\n\0";

/*Color naranja*/
const char* fragmentShader4Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.98f, 0.55f, 0.25f, 1.0f);\n"
"}\n\0";

/*Color Rojo*/
const char* fragmentShader5Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00f, 0.30f, 0.30f, 1.0f);\n"
"}\n\0";

/*Color morado*/
const char* fragmentShader6Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.36f, 0.44f, 0.83f, 1.0f);\n"
"}\n\0";

/*Color azul*/
const char* fragmentShader7Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.22f, 0.69f, 0.93f, 1.0f);\n"
"}\n\0";

int main()
{
    /* Inicializa la libreria GLFW*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Crear una ventana y su contexto OpenGL */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1P2 Gary Campana", NULL, NULL);
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

            /*CONSTRUIR Y COMPILAR EL SHADERPROGRAM*/
    //**NOTA: Se elimina la comprobación de errores para reducir las líneas de código

    /*Creación de un objeto shader referenciado por un ID*/
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    /*Salida de colores*/
    unsigned int fragmentShaderVerdeOscuro = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderAmarillo = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShaderVerdeClaro = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderNaranja = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShaderRojo = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShaderMorado = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShaderAzul = glCreateShader(GL_FRAGMENT_SHADER); 
    /*Creación shaderProgram*/
    unsigned int shaderProgramVerdeOscuro = glCreateProgram();
    unsigned int shaderProgramAmarillo = glCreateProgram();
    unsigned int shaderProgramVerdeClaro = glCreateProgram();
    unsigned int shaderProgramNaranja = glCreateProgram();
    unsigned int shaderProgramRojo = glCreateProgram();
    unsigned int shaderProgramMorado = glCreateProgram();
    unsigned int shaderProgramAzul = glCreateProgram();
    
    //Compilación del vertexShader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Compilación de los fragmentShader
    glShaderSource(fragmentShaderVerdeOscuro, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderVerdeOscuro);

    glShaderSource(fragmentShaderAmarillo, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderAmarillo);

    glShaderSource(fragmentShaderVerdeClaro, 1, &fragmentShader3Source, NULL);
    glCompileShader(fragmentShaderVerdeClaro);

    glShaderSource(fragmentShaderNaranja, 1, &fragmentShader4Source, NULL);
    glCompileShader(fragmentShaderNaranja);

    glShaderSource(fragmentShaderRojo, 1, &fragmentShader5Source, NULL);
    glCompileShader(fragmentShaderRojo);

    glShaderSource(fragmentShaderMorado, 1, &fragmentShader6Source, NULL);
    glCompileShader(fragmentShaderMorado);

    glShaderSource(fragmentShaderAzul, 1, &fragmentShader7Source, NULL);
    glCompileShader(fragmentShaderAzul);

    //Compilación de los shaderProgram
    glShaderSource(shaderProgramVerdeOscuro, 1, &fragmentShader1Source, NULL);
    glCompileShader(shaderProgramVerdeOscuro);

    glShaderSource(shaderProgramAmarillo, 1, &fragmentShader2Source, NULL);
    glCompileShader(shaderProgramAmarillo);

    glShaderSource(shaderProgramVerdeClaro, 1, &fragmentShader3Source, NULL);
    glCompileShader(shaderProgramVerdeClaro);

    glShaderSource(shaderProgramNaranja, 1, &fragmentShader4Source, NULL);
    glCompileShader(shaderProgramNaranja);

    glShaderSource(shaderProgramRojo, 1, &fragmentShader5Source, NULL);
    glCompileShader(shaderProgramRojo);

    glShaderSource(shaderProgramMorado, 1, &fragmentShader6Source, NULL);
    glCompileShader(shaderProgramMorado);

    glShaderSource(shaderProgramAzul, 1, &fragmentShader7Source, NULL);
    glCompileShader(shaderProgramAzul);

    //Enlaces
    glAttachShader(shaderProgramVerdeOscuro, vertexShader);
    glAttachShader(shaderProgramVerdeOscuro, fragmentShaderVerdeOscuro);
    glLinkProgram(shaderProgramVerdeOscuro);

    glAttachShader(shaderProgramAmarillo, vertexShader);
    glAttachShader(shaderProgramAmarillo, fragmentShaderAmarillo);
    glLinkProgram(shaderProgramAmarillo);

    glAttachShader(shaderProgramVerdeClaro, vertexShader);
    glAttachShader(shaderProgramVerdeClaro, fragmentShaderVerdeClaro);
    glLinkProgram(shaderProgramVerdeClaro);

    glAttachShader(shaderProgramNaranja, vertexShader);
    glAttachShader(shaderProgramNaranja, fragmentShaderNaranja);
    glLinkProgram(shaderProgramNaranja);

    glAttachShader(shaderProgramRojo, vertexShader);
    glAttachShader(shaderProgramRojo, fragmentShaderRojo);
    glLinkProgram(shaderProgramRojo);

    glAttachShader(shaderProgramMorado, vertexShader);
    glAttachShader(shaderProgramMorado, fragmentShaderMorado);
    glLinkProgram(shaderProgramMorado);

    glAttachShader(shaderProgramAzul, vertexShader);
    glAttachShader(shaderProgramAzul, fragmentShaderAzul);
    glLinkProgram(shaderProgramAzul);

    //Datos de los vértices 
    float vertices[] = {
       0.0f, 0.0f, 0.0f, // P1 --> índice 0 -> A
       0.9f, 0.0f, 0.0f, // P2 --> índice 1 -> B
        0.35f, -0.35f, 0.0f, // P3 --> índice 2 -> C
       0.7f, -0.7f, 0.0f, // P4 --> índice 3 -> D
       0.0f , -0.7f, 0.0f, // P5 --> índice 4 -> E
       -0.4f, -0.4f, 0.0f, // P6 --> índice 5 -> F
        -0.1f, -0.1f, 0.0f, // P7 --> índice 6 -> G
       -0.4f, 0.2f, 0.0f,  // P8 --> índice 7 -> H
        0.2f, 0.2f, 0.0f, // P9 --> índice 8 -> I 
       -0.1f, 0.5f, 0.0f, // P10--> índice 9 -> J 
       -0.1f, 0.6f, 0.0f, // P11--> índice10 -> K 
        -0.6f, 0.6f, 0.0f, // P12--> índice11 -> L 
        -0.1f, 1.0f, 0.0f, // P13--> índice12 -> M 
        0.2f, 0.7f, 0.0f // P14--> índice13 -> N 
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
    //Enlaza el objeto de matriz de vértices, luego enlaza y establece el búfer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Establacimiento de los punteros de los atributos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Entrada
        processInput(window);

        // Renderización (Color de fondo)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO); //Vinculación VAO

        //Dibujar los triángulos
        glUseProgram(shaderProgramVerdeOscuro);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgramAmarillo);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(float)));

        glUseProgram(shaderProgramVerdeClaro);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));

        glUseProgram(shaderProgramNaranja);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(float)));



        glUseProgram(shaderProgramRojo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(float)));



        glUseProgram(shaderProgramMorado);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(float)));

        glUseProgram(shaderProgramAzul);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(24 * sizeof(float)));



        /*Intercambia los buffers delantero y trasero de la ventana especificada*/
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*Opcional: Eliminar los objetos que fueron enlazados*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //terminan, borrando todos los recursos GLFW previamente asignados.glfwTerminate();
    glfwTerminate();
    return 0;
}

    /*Si se seleciona la tecla esc, se cierra la ventana*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

    /*Las coordenadas normalizadas se transformarán en coordenadas de espacio de pantalla*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}