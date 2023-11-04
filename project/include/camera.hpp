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
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
        virtual ~Camera();

        void processInput(GLFWwindow *window, float);
        void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        void setCamPos(glm::vec3 position) { this->cam_pos = position; }

        glm::vec3 getCamPos() { return this->cam_pos; }
        glm::vec3 getCamFront() { return this->cam_front; }
        glm::vec3 getCamTarget() { return this->cam_target; }
        glm::vec3 getUpVector() { return this->up; }

        float getFov() { return this->fov; }
        float getYaw() { return this->yaw; }
        float getPitch() { return this->pitch; }

        glm::mat4 getPerspectiveMatrix(int width, int height, float near = 0.1f, float far = 100.0f);
        glm::mat4 getViewMatrix();
    protected:
        glm::vec3 cam_pos;
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
