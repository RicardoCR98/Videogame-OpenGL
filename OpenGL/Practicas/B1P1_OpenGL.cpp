//Se incluye la biblioteca GLFW, con esta creamos ventanas, manejamos eventos.
#include <GLFW/glfw3.h>

int main(void)
{
    //Declaramos la variable window que representa la variable de la aplicación
    GLFWwindow* window;

    //inicializamos la biblioteca GLFW. 
    //Si falla, el programa devuelve un código de error (-1)
    if (!glfwInit())
        return -1;

    // Creamos una ventana con cierta dimension y titulo, y le asignamos a la variable window
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    //Si la ventana llegase a fallar, llamamos al metodo glfwterminate para liberar recursos y devolver -1.
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    //Decimos que en esta ventana realizaremos las operaciones de dibujo
    glfwMakeContextCurrent(window);

    //Iniciamos el bucle que se ejecutará hasta que el usuario cierre la ventana
    //en cada iteración ocurre lo siguiente:
    while (!glfwWindowShouldClose(window))
    {
        //borra el bufer de color para preparar el siguiente cuadro
        glClear(GL_COLOR_BUFFER_BIT);

        //intercambia los buferes frontal y trasero para mostrar el resultado del dibujo en pantalla
        // 2 buffers, la primera es la frontal (visualización) y el segundo buffer es el trasero (dibujo)
        glfwSwapBuffers(window);

        //Esta función procesa eventos de entrada (mouse, teclado, etc)
        // Si no se llama a esta función, la ventana se bloquearía y no respondería a la interacción del usuario.
        glfwPollEvents();
    }
    //liberamos los recursos de GLFW y retorna 0 indicando que no hay fallas al SO
    glfwTerminate();
    return 0;
}

/*En resumen, este código inicializa GLFW, crea una ventana, establece el contexto de OpenGL,
y luego ejecuta un bucle en el que se borra el búfer de color, 
se intercambian los búferes, 
se procesan los eventos y se repite hasta que se cierre la ventana*/