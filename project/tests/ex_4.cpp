#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int _WIDTH = 800;
int _HEIGHT = 600;

void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f);

// moving
float fov = 45.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = 0;
float last_y = 0;

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

int run_004(const int width, const int height)
{
    _WIDTH = width;
    _HEIGHT = height;

    _stbi_set_flip_vertically_on_load(true);

    Texture2D *texture_0 = new Texture2D("resources/textures/container.jpg");
    Texture2D *texture_1 = new Texture2D("resources/textures/awesomeface.png", true);

    Shader *shader = new Shader("glsl/test_2_vertex_shader.vs", "glsl/test_2_fragment_shader.fs");
    SObject *cube = createCube();

    shader->use();
    shader->setInt("texture0", 0);
    shader->setInt("texture1", 1);

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_front, up);

        shader->use();

        shader->setMatrix4fv("view", glm::value_ptr(view));
        shader->setMatrix4fv("projection", glm::value_ptr(projection));

        texture_0->bind(GL_TEXTURE0);
        texture_1->bind(GL_TEXTURE1);

        cube->_vao->bind();
        cube->_vbo->bind();
        cube->_ebo->bind();

        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * (i + 1) * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 1.0, 1.0));

            shader->setMatrix4fv("model", glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        processInput(window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete texture_0;
    delete texture_1;

    delete shader;
    delete cube;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cam_speed = static_cast<float>(2.5 * delta_time);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam_pos += cam_speed * cam_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam_pos -= cam_speed * cam_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam_pos -= glm::normalize(glm::cross(cam_front, up)) * cam_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam_pos += glm::normalize(glm::cross(cam_front, up)) * cam_speed;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static bool first = true;
    if (first) {
        last_x = xpos;
        last_y = ypos;
        first = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cam_front = glm::normalize(direction);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
