#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// camera
//Camera camera(glm::vec3(25.0f, 6.0f, -65.0f));
Camera camera(glm::vec3(-20.0f,3.0f,3.0f));

//Mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Tiempo
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


//linterna encender apagar
bool encendido = false;


// ########## Fantasma #######################
// Define las propiedades de la órbita
float centerX = 0.0f; // Posicion en X del fantasma
float centerZ = -50.0f; // Posicion en Z del fantasma
float radius = 20.0f; // Radio del circula del recorrido del fantasma
float angularSpeed = 20.0f; // Velocidad del fantasma
float ghostYOffset = -5.0f;//altura del fantasma inicial

// Variables globales para la animación del fantasma
float ghostVerticalAmplitude = 10.0f; // movimiento vertical del fantasma
float ghostVerticalFrequency = 3.0f; // Ajusta la frecuencia del movimiento vertical
float ghostVerticalOffset = 0.0f; // Offset vertical inicial del fantasma
bool moveGhostUp = true; // Bandera para controlar la dirección del movimiento


//########## Variables para la pequeña animación de la mascara #############
bool isAnimating = false;
float animationStartTime = 0.0f;
float animationDuration = 5.0f; // Duración de la animación en segundos
glm::vec3 maskStartPosition;
float maskStartRotation;
bool moveMaskForward = false; // Indica si la máscara debe moverse hacia adelante
bool moveMaskBackward = false; // Indica si la máscara debe moverse hacia atrás

// Define las coordenadas de la posición inicial y la posición "x" deseada
glm::vec3 maskInitialPosition = glm::vec3(220.0f, 0.0f, -35.0f);
glm::vec3 maskTargetPosition = glm::vec3(-20.0f, 0.0f, -35.0f); // Posición "x" deseada
glm::vec3 maskTargetPosition2 = glm::vec3(-220.0f, 0.0f, -35.0f); // Posición "x" deseada
float moveSpeedMask = 200.0f; // Ajusta la velocidad de movimiento según sea necesario

//Variables de la lampara
float frequency = 5.0f; // Controla la velocidad general del parpadeo
float intensity = 2.0f; //

int main()
{
    // se iniciliza y configura GLWF
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creación del objeto ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Examen", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Crear el contexto de la ventana
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//Llamar a la funcion en cada cambio de ventana
    glfwSetCursorPosCallback(window, mouse_callback); //posición del cursor
    glfwSetScrollCallback(window, scroll_callback); //desplazamiento

    //decirle a GLFW que capture nuestro ratón
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicializar glad antes de llamar a las funciones
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "La inicilizacion de glad a fallado" << std::endl;
        return -1;
    }

    // configurar el estado global de opengl
    glEnable(GL_DEPTH_TEST);

    // Construir y compilar shaders
    Shader ourShader("shaders/VS_Examen.vs", "shaders/FS_Examen.fs");

    // Carga de modelos
    Model map("model/map/Map.obj");
    Model lamparas("model/lamparas/lamparas.obj");
    Model level("model/level/level.obj");
    Model ghost("model/ghost/ghost.obj");
    Model mask("model/mask/mask.obj");
    Model hand_monster("model/hand_monster/mounstruo1.obj");
    Model cavern("model/caverna/cavern.obj");
    Model baul("model/baul/baul.obj");

    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);

    // Render loop

    while (!glfwWindowShouldClose(window))
    {

        //devuelve el tiempo actual
        float currentTime = glfwGetTime();
        // Calcular la posición en el círculo (varaibles del fantasma)
        float xPos = centerX + radius * glm::cos(glm::radians(angularSpeed * currentTime));
        float zPos = centerZ + radius * glm::sin(glm::radians(angularSpeed * currentTime));
        //Sensacion de flote del fantasma
        float yOffset1 = ghostVerticalAmplitude * sin(ghostVerticalFrequency);

        //Devuelve el frame actual
        float currentFrame = glfwGetTime() + 1;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Define un patrón de parpadeo específico
        float pattern = fmod(currentFrame * frequency, 20.0f);
        if (pattern < 1.0f) {
            // Primer período rápido
            intensity = 1.0f;
        }
        else if (pattern < 2.0f) {
            // Segundo período rápido
            intensity = 0.0f;
        }
        else if (pattern < 3.0f) {
            // Tercer período rápido
            intensity = 1.0f;
        }
        else if (pattern < 7.0f) {
            // Período lento
            intensity = 1.0f;
        }
        else {
            // Sexto período rápido
            intensity = 0.0f;
        }

        // Input
        processInput(window);

        // Renderizado
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activa el sombreador cuando se configuren objetos uniformes/de dibujo
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setFloat("material.shininess", 32.0f);

        // Direccional light ESCENA
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05);
        ourShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

        // Point light Toda la escena
        //ourShader.setVec3("pointLights[0].position", 1.0f, 1.0f, 1.0f);
        ////ourShader.setVec3("pointLights[0].ambient", 0.5f * intensity, 0.5f * intensity, 0.5f * intensity);
        //ourShader.setVec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
        //ourShader.setVec3("pointLights[0].diffuse", 0.2f, 0.2f, 0.2f);
        //ourShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
        //ourShader.setFloat("pointLights[0].constant", 1.0f);
        //ourShader.setFloat("pointLights[0].linear", 0.000);
        //ourShader.setFloat("pointLights[0].quadratic", 0.00000);
        
        // Point light Toda la escena
        ourShader.setVec3("pointLights[0].position", 1.0f, 1.0f, 1.0f); // La posición de la luz puntual (ajústala según tus necesidades)
        ourShader.setVec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f); // Componente ambiental tenue
        ourShader.setVec3("pointLights[0].diffuse", 0.2f, 0.2f, 0.2f); // Componente difusa tenue
        ourShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f); // Componente especular apagada (no hay reflejos intensos)
        ourShader.setFloat("pointLights[0].constant", 1.0f); // Atenuación constante (luz no se atenúa rápidamente)
        ourShader.setFloat("pointLights[0].linear", 0.001); // Atenuación lineal moderada
        ourShader.setFloat("pointLights[0].quadratic", 0.000005); // Atenuación cuadrática más rápida


        // spotLight 1 Camara      
        ourShader.setVec3("spotLight[0].position", camera.Position);
        ourShader.setVec3("spotLight[0].direction", camera.Front);
        if (encendido) {
            ourShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
            ourShader.setVec3("spotLight[0].diffuse", 0.0f, 0.0f, 0.0f);
            ourShader.setVec3("spotLight[0].specular", 0.0f, 0.0f, 0.0f);
        }
        else {
            ourShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
            ourShader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
            ourShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
        }
        ourShader.setFloat("spotLight[0].constant", 1.0f);
        ourShader.setFloat("spotLight[0].linear", 0.001);
        ourShader.setFloat("spotLight[0].quadratic", 0.00007);
        ourShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));

        // spotLight 2 lamparas [1]
        ourShader.setVec3("spotLight[1].position", glm::vec3(-20.0f, 10.0f, -35.0f)); // Ajusta la altura para simular una farola en una calle alta
        ourShader.setVec3("spotLight[1].direction", glm::vec3(0.0f, -1.0f, 0.0f)); // La luz apunta hacia abajo
        ourShader.setVec3("spotLight[1].ambient", 0.3f , 0.3f , 0.3f ); // Componente ambiental tenue
        ourShader.setVec3("spotLight[1].diffuse", 0.8f * intensity, 0.8f * intensity, 0.8f * intensity); // Componente difusa tenue
        ourShader.setVec3("spotLight[1].specular", 0.8f, 0.8f, 0.8f); // Componente especular tenue
        ourShader.setFloat("spotLight[1].constant", 1.0f);
        ourShader.setFloat("spotLight[1].linear", 0.001); // Atenuación lineal más rápida
        ourShader.setFloat("spotLight[1].quadratic", 0.00007); // Atenuación cuadrática más rápida
        ourShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(20.0f))); // A mas pequeño mas estrecho
        ourShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(60.0f))); // A mas pequeño mas estrecho



        // spotLight 2 lamparas [2]
        ourShader.setVec3("spotLight[2].position", glm::vec3(-20.0f, 19.0f, -65.0f)); // Ajusta la altura para simular una farola en una calle alta
        ourShader.setVec3("spotLight[2].direction", glm::vec3(0.0f, -1.0f, 0.0f)); // La luz apunta hacia abajo
        ourShader.setVec3("spotLight[2].ambient", 0.3f, 0.3f, 0.3f); // Componente ambiental tenue
        ourShader.setVec3("spotLight[2].diffuse", 0.8f * intensity, 0.8f * intensity, 0.8f * intensity);
        ourShader.setVec3("spotLight[2].specular", 0.4f, 0.4f, 0.4f);
        ourShader.setFloat("spotLight[2].constant", 1.0f);
        ourShader.setFloat("spotLight[2].linear", 0.001);
        ourShader.setFloat("spotLight[2].quadratic", 0.00007);
        ourShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(20.0f))); // Hazlo muy estrecho
        ourShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(60.0f))); // Hazlo aún más estrecho

        // spotLight 2 lamparas [3]
        ourShader.setVec3("spotLight[3].position", glm::vec3(20.0f, 10.0f, -67.0f)); // Posición de la farola
        ourShader.setVec3("spotLight[3].direction", glm::vec3(0.0f, -1.0f, 0.0f)); // La luz apunta hacia abajo
        ourShader.setVec3("spotLight[3].ambient", 1.0f, 0.0f, 0.0f); 
        ourShader.setVec3("spotLight[3].diffuse", 1.0f*intensity, 0.0f, 0.0f); 
        ourShader.setVec3("spotLight[3].specular", 1.0f, 0.0f, 0.0f); 
        ourShader.setFloat("spotLight[3].constant", 1.0f); // Atenuación constante
        ourShader.setFloat("spotLight[3].linear", 0.001); // Atenuación lineal moderada
        ourShader.setFloat("spotLight[3].quadratic", 0.00007); // Atenuación cuadrática más rápida
        ourShader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(20.0f))); // A mas pequeño mas estrecho
        ourShader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(60.0f))); // A mas pequeño mas estrecho



        // spotLight 2 lamparas [4]
        ourShader.setVec3("spotLight[4].position", glm::vec3(20.0f, 19.0f, -35.0f)); // Posición de la farola
        ourShader.setVec3("spotLight[4].direction", glm::vec3(0.0f, -1.0f, 0.0f)); // La luz apunta hacia abajo
        ourShader.setVec3("spotLight[4].ambient", 0.1f, 0.1f, 0.1f); // Componente ambiental tenue
        ourShader.setVec3("spotLight[4].diffuse", 0.8f, 0.8f, 0.8f); // Componente difusa
        ourShader.setVec3("spotLight[4].specular", 1.0f, 1.0f, 1.0f); // Componente especular
        ourShader.setFloat("spotLight[4].constant", 1.0f); // Atenuación constante
        ourShader.setFloat("spotLight[4].linear", 0.001); // Atenuación lineal
        ourShader.setFloat("spotLight[4].quadratic", 0.00007); // Atenuación cuadrática
        ourShader.setFloat("spotLight[4].cutOff", glm::cos(glm::radians(20.0f))); // Hazlo muy estrecho
        ourShader.setFloat("spotLight[4].outerCutOff", glm::cos(glm::radians(60.0f))); // Hazlo aún más estrecho


        // Transformaciones de vista/proyección
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);


        // #################### Lamparas separadas ######################
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, -50.0f));
        model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", model1);
        lamparas.Draw(ourShader);

        //################ El modelo Level ########################
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(75.0f, -1.2f, -35.0f));
        model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", model2);
        level.Draw(ourShader);


        //################### El modelo 'ghost' #########################

        // Controlar la dirección vertical del movimiento del fantasma
        if (moveGhostUp)
        {
            ghostVerticalOffset += yOffset1 * deltaTime;
        }
        else
        {
            ghostVerticalOffset -= yOffset1 * deltaTime;
        }

        // Cambiar la dirección si el fantasma alcanza cierta altura
        if (ghostVerticalOffset > 1.0f)
        {
            moveGhostUp = false;
        }
        else if (ghostVerticalOffset < -1.0f)
        {
            moveGhostUp = true;
        }
        // Actualizar la posición vertical del modelo 'ghost'

        glm::vec3 ghostPosition(xPos, ghostYOffset + ghostVerticalOffset, zPos);
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, ghostPosition);
        model3 = glm::scale(model3, glm::vec3(3.0f, 3.0f, 3.0f));
        ourShader.setMat4("model", model3);
        ghost.Draw(ourShader);



        // ################### El modelo 'mask' ############################

        // Dentro del bucle principal
        if (isAnimating) {
            float currentTime = glfwGetTime();
            float elapsedTime = currentTime - animationStartTime;

            if (elapsedTime >= animationDuration) {
                // La animación ha terminado
                isAnimating = false;
                maskInitialPosition = maskTargetPosition;
            }
            else {
                // Mantener la rotación en -90 grados durante 5 segundos
                float maskRotation = 0.0f;

                // Si han pasado 5 segundos, restablece la rotación a 0 grados
                if (elapsedTime >= 5.0f) {
                    maskRotation = 0.0f;
                }

                // Interpolar la posición de la máscara
                maskInitialPosition = glm::mix(maskStartPosition, maskTargetPosition, elapsedTime / animationDuration);

                // Actualizar la posición y la rotación del modelo 'mask'
                glm::mat4 model4_1 = glm::mat4(1.0f);
                model4_1 = glm::translate(model4_1, maskInitialPosition);
                model4_1 = glm::rotate(model4_1, glm::radians(maskRotation), glm::vec3(0.0f, 1.0f, 0.0f));
                model4_1 = glm::scale(model4_1, glm::vec3(40.0f, 40.0f, 40.0f));
                ourShader.setMat4("model", model4_1);
                mask.Draw(ourShader);
            }
        }
        else {
            if (moveMaskForward) {
                // Calcula la dirección y la distancia hacia la nueva posición "x"
                glm::vec3 moveDirection = glm::normalize(maskTargetPosition2 - maskInitialPosition);
                float distance = glm::distance(maskInitialPosition, maskTargetPosition2);

                // Calcula el desplazamiento para el frame actual
                float frameMovement = moveSpeedMask * deltaTime;

                // Comprueba si el modelo ha alcanzado o superado la posición "x"
                if (distance <= frameMovement) {
                    // Si el modelo ha llegado o superado la posición "x", detén el movimiento
                    maskInitialPosition = maskTargetPosition2;
                    moveMaskForward = false; // Detener el movimiento hacia adelante
                }
                else {
                    // Aplica el desplazamiento al modelo 'mask'
                    maskInitialPosition += moveDirection * frameMovement;
                }
            }
            else if (moveMaskBackward) {
                // Calcula la dirección y la distancia hacia la posición inicial
                glm::vec3 moveDirection = glm::normalize(maskInitialPosition - maskTargetPosition2);
                float distance = glm::distance(maskTargetPosition2, maskInitialPosition);

                // Calcula el desplazamiento para el frame actual
                float frameMovement = moveSpeedMask * deltaTime;

                // Comprueba si el modelo ha alcanzado o superado la posición inicial
                if (distance <= frameMovement) {
                    // Si el modelo ha llegado o superado la posición inicial, detén el movimiento
                    maskInitialPosition = maskTargetPosition2;
                    moveMaskBackward = false; // Detener el movimiento hacia atrás
                }
                else {
                    // Aplica el desplazamiento al modelo 'mask' (en sentido contrario)
                    maskInitialPosition += moveDirection * frameMovement;
                }
            }
            // No se está ejecutando la animación, simplemente renderiza la máscara en su posición actual
            glm::mat4 model4 = glm::mat4(1.0f);
            model4 = glm::translate(model4, maskInitialPosition);
            model4 = glm::rotate(model4, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model4 = glm::scale(model4, glm::vec3(40.0f, 40.0f, 40.0f));
            ourShader.setMat4("model", model4);
            mask.Draw(ourShader);
        }


        //####################################################################
        // ################### Escenario ################################
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -50.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", model);
        map.Draw(ourShader);

        glm::vec3 cameraPosition = camera.Position;
        glm::vec3 model5Position = glm::vec3(-20.0f, 3.0f, -80.0f);
        float distance = glm::length(model5Position - cameraPosition);

        if (distance < 5.0f) {
            glClearColor(0.1f, 0.0f, 0.2f, 1.0f); // Color de fondo morado
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //Model 5
            glm::mat4 model5 = glm::mat4(1.0f);
            model5 = glm::translate(model5, glm::vec3(-20.0f, 3.0f, -60.0f));
            model5 = glm::rotate(model5, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model5 = glm::scale(model5, glm::vec3(3.0f, 3.0f, 3.0f));
            ourShader.setMat4("model", model5);
            cavern.Draw(ourShader);


            //Model 6
            glm::mat4 model6 = glm::mat4(1.0f);
            model6 = glm::translate(model6, glm::vec3(-20.0f, 3.0f, -60.0f));
            model6 = glm::rotate(model6, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model6 = glm::scale(model6, glm::vec3(0.18f, 0.18f, -0.18f));
            ourShader.setMat4("model", model6);
            baul.Draw(ourShader);
        }
        else {
            map.Draw(ourShader);
        }

        //Model7
        glm::mat4 model7 = glm::mat4(1.0f);
        model7 = glm::translate(model7, glm::vec3(-20.0f, 3.0f, -80.0f));
        model7 = glm::rotate(model7, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model7 = glm::scale(model7, glm::vec3(2.5f, 2.5f, 2.5f));

        ourShader.setMat4("model", model7);
        hand_monster.Draw(ourShader);

        // Intercambia los búferes y gestiona los eventos de E/S (teclas presionadas/liberadas, movimiento del ratón, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Termina GLFW, liberando todos los recursos previamente asignados por GLFW
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 15 * deltaTime;
    float walkingAmplitude = 0.2f; // Ajusta la amplitud del movimiento al caminar
    float walkingFrequency = 10.0f; // Ajusta la frecuencia del movimiento al caminar

    // Calcula el movimiento vertical basado en una función seno
    float yOffset = walkingAmplitude * sin(glfwGetTime() * walkingFrequency);
    //Para W y S
    glm::vec3 forwardMovement = cameraSpeed * camera.Front;
    //Sensacion de movimiento de arriba hacia abajo
    glm::vec3 verticalMovement(0.0f, yOffset * 0.03, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += forwardMovement + verticalMovement;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= forwardMovement + verticalMovement;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed + verticalMovement;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed + verticalMovement;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) { //linterna
        encendido = true;
        ghostYOffset = -4.0f;
    }
    else {
        encendido = false;
        ghostYOffset = -100.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        moveMaskForward = true;
    }
    else {
        moveMaskForward = false;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        moveMaskBackward = true;
    }
    else {
        moveMaskBackward = false;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        // Iniciar la animación si no está en curso
        if (!isAnimating) {
            isAnimating = true;
            animationStartTime = glfwGetTime();
            maskStartPosition = maskInitialPosition;
            maskStartRotation = -90.0f;
        }
    }

}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


