#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement.
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float walkingAmplitude = 0.05f;
const float walkingFrequency = 5.0f;

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        BoundingBoxWidth = 1.0f; // Ajusta el ancho del bounding box de la cámara
        BoundingBoxHeight = 1.0f; // Ajusta la altura del bounding box de la cámara
        BoundingBoxDepth = 1.0f; // Ajusta la profundidad del bounding box de la cámara
        updateCameraVectors();
    }

    // Devuelve la matriz de vista calculada utilizando Euler Angles y la matriz LookAt
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Procesa la entrada del teclado
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;

        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

    }

    // Procesa la entrada del mouse
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    // Procesa la entrada de la rueda del mouse
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    // Obtener el ancho del bounding box de la cámara
    float GetBoundingBoxWidth() const {
        return BoundingBoxWidth;
    }

    // Obtener la altura del bounding box de la cámara
    float GetBoundingBoxHeight() const {
        return BoundingBoxHeight;
    }

    // Obtener la profundidad del bounding box de la cámara
    float GetBoundingBoxDepth() const {
        return BoundingBoxDepth;
    }

    // Obtener la posición anterior de la cámara
    glm::vec3 GetPreviousPosition() const {
        return PreviousPosition;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
        PreviousPosition = Position;
    }

    float BoundingBoxWidth;
    float BoundingBoxHeight;
    float BoundingBoxDepth;
    glm::vec3 PreviousPosition;
};

#endif
