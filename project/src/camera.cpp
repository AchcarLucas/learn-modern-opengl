#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 target)
{
    this->setCamPos(position);
    this->lookAt(target);
}

void Camera::lookAt(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - this->getCamPos());

    /// calculate pitch (up - down)
    this->pitch = glm::degrees(glm::asin(direction.y));
    /// calculate yaw (right - left)
    this->yaw = glm::degrees(std::atan2(direction.x, -direction.z)) - 90.0f;
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

    float cam_speed = static_cast<float>(speed * delta_time);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->cam_pos += cam_speed * this->cam_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->cam_pos -= cam_speed * this->cam_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->cam_pos -= glm::normalize(this->getRightVector()) * cam_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->cam_pos += glm::normalize(this->getRightVector()) * cam_speed;
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

glm::mat4 Camera::getPerspectiveMatrix(int width, int height, float near, float far)
{
    return glm::perspective(glm::radians(getFov()), (float)width / (float)height, near, far);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(getCamPos(), getCamPos() + getCamFront(), getUpVector());
}
