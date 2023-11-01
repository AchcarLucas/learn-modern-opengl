#include "camera.hpp"

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::processInput(GLFWwindow *window, float delta_time)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cam_speed = static_cast<float>(2.5 * delta_time);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->cam_pos += cam_speed * this->cam_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->cam_pos -= cam_speed * this->cam_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->cam_pos -= glm::normalize(glm::cross(this->cam_front, this->up)) * cam_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->cam_pos += glm::normalize(glm::cross(this->cam_front, this->up)) * cam_speed;
}

void Camera::mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    static bool FIRST = true;

    static float SENSITIVITY = 0.1f;
    static float MAX_ANGLE = 89.0f;

    if (FIRST) {
        this->last_x = x_pos;
        this->last_y = y_pos;
        FIRST = false;
    }

    float x_offset = x_pos - this->last_x;
    float y_offset = this->last_y - y_pos;

    this->last_x = x_pos;
    this->last_y = y_pos;

    x_offset *= SENSITIVITY;
    y_offset *= SENSITIVITY;

    this->yaw   += x_offset;
    this->pitch += y_offset;

    if(this->pitch > MAX_ANGLE)
        this->pitch = MAX_ANGLE;
    if(this->pitch < -MAX_ANGLE)
        this->pitch = -MAX_ANGLE;

    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->cam_front = glm::normalize(direction);
}

void Camera::scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    static float MAX_FOV = 45.0f;
    static float MIN_FOV = 1.0f;

    this->fov -= (float)y_offset;

    if (this->fov < MIN_FOV)
        this->fov = MIN_FOV;
    if (this->fov > MAX_FOV)
        this->fov = MAX_FOV;
}
