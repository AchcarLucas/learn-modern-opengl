#include "init.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "vbo.hpp"
#include "ubo.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static void processInput(GLFWwindow *, float);
static void mouseCallback(GLFWwindow*, double, double);
static void scrollCallback(GLFWwindow*, double, double);

// camera class
static Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 155.0f));

// timing
static float delta_time = 0.0f;	// time between current frame and last frame
static float last_frame = 0.0f;

int run_028(const int width, const int height)
{
    _stbi_set_flip_vertically_on_load(true);

    camera->setCamPos(glm::vec3(0.0f, 0.0f, 155.0f));
    camera->setCamSpeed(50.0f);

    Shader *shader_asteroid = new Shader("glsl/ex_28/rock.vs", "glsl/ex_28/rock.fs");
    Shader *shader_planet = new Shader("glsl/ex_25/model_loading.vs", "glsl/ex_25/model_loading.fs");

    Model *model_asteroid = new Model("./resources/obj/rock/rock.obj");
    Model *model_planet = new Model("./resources/obj/planet/planet.obj");

    glm::mat4 projection = camera->getPerspectiveMatrix(width, height, 0.1f, 1000.0f);
    glm::mat4 view = camera->getViewMatrix();

    UBO *ubo_matrices = new UBO("Matrices", 2 * sizeof(glm::mat4), 0);
    UBO *ubo_global = new UBO("Global", 1 * sizeof(float), 1);

    ubo_matrices->UBOSubBuffer(glm::value_ptr(projection), 0, sizeof(glm::mat4));
    ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

    shader_asteroid->setUniformBlockBinding(ubo_matrices->getName(), ubo_matrices->getBinding());
    shader_asteroid->setUniformBlockBinding(ubo_global->getName(), ubo_global->getBinding());

    unsigned int amount_model = 10000;
    glm::mat4 *mat_model_asteroid = new glm::mat4[amount_model];

    {
        srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

        float radius = 150.0;
        float offset = 25.0f;

        for (unsigned int i = 0; i < amount_model; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)amount_model * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: Scale between 0.05 and 0.25f
            float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            mat_model_asteroid[i] = model;
        }
    }

    VBO *vbo_array = new VBO();

    {
        vbo_array->bind();
        vbo_array->VBOBuffer(&mat_model_asteroid[0], sizeof(glm::mat4) * amount_model, GL_STATIC_DRAW);

        // vamos primeiro fazer o bind no nosso VAO da mesh
        std::vector<Mesh *> meshes = model_asteroid->getMeshes();

        for (auto& it: meshes) {
            it->getVAO()->bind();
            vbo_array->VBOBufferDivisor(3, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
            vbo_array->VBOBufferDivisor(4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
            vbo_array->VBOBufferDivisor(5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            vbo_array->VBOBufferDivisor(6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
            it->getVAO()->unbind();
        }

        vbo_array->unbind();
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glm::mat4 view = camera->getViewMatrix();
        ubo_matrices->UBOSubBuffer(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));
        ubo_global->UBOSubBuffer(&current_frame, 0, sizeof(float));

        // planet
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

            shader_planet->use();
            shader_planet->setMatrix4fv("model", glm::value_ptr(model));
            model_planet->draw(shader_planet);
        }

        // asteroid
        {
            shader_asteroid->use();
            model_asteroid->draw(shader_asteroid, amount_model);
        }

        processInput(window, delta_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    delete shader_asteroid;

    delete ubo_matrices;
    delete ubo_global;

    delete model_asteroid;
    delete model_planet;

    delete vbo_array;

    delete mat_model_asteroid;

    glfwTerminate();
    return 0;
}

static void processInput(GLFWwindow *window, float delta_time)
{
    camera->processInput(window, delta_time);
}

static void mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera->mouseCallback(window, x_pos, y_pos);
}

static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera->scrollCallback(window, x_offset, y_offset);
}
