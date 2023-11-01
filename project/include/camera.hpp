#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        void processInput(GLFWwindow *window, float);
        void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        glm::vec3 getCamPos() { return this->cam_pos; }
        glm::vec3 getCamFront() { return this->cam_front; }
        glm::vec3 getCamTarget() { return this->cam_target; }
        glm::vec3 getUpVector() { return this->up; }

        float getFov() { return this->fov; }
        float getYaw() { return this->yaw; }
        float getPitch() { return this->pitch; }
    protected:
        glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f);

        float fov = 45.0f;
        float yaw = -90.0f;
        float pitch = 0.0f;

        float last_x = 0;
        float last_y = 0;

    private:
};

#endif // CAMERA_HPP
