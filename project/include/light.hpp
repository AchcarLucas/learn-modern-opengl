#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <shader.hpp>

class Light
{
    public:
        Light(glm::vec3 position, const float near_plane, const float far_plane);
        virtual ~Light();

        void setPosition(glm::vec3 position) { this->position = position; }

        glm::vec3 getPosition() { return position; }
        float getNearPlane() { return this->near_plane; }
        float getFarPlane() { return this->far_plane; }

        virtual void settingShader(Shader *_shader) { }

    protected:
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 position;
        float near_plane;
        float far_plane;

    private:
};

class DirectionalLight : public Light
{
    public:
        DirectionalLight(const glm::vec3 position, const glm::vec3 direction, const float box_size, const float near, const float far);
        virtual ~DirectionalLight();

        void setDirection(glm::vec3 direction) { this->direction = direction; }

        void settingShader(Shader *_shader);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getLightSpaceMatrix() { return this->getProjectionMatrix() * this->getViewMatrix(); }

        float getBoxSize() { return this->box_size; }

    protected:
        float box_size;

        glm::vec3 direction;

    private:
};

class PointLight : public Light
{
    public:
        PointLight(const glm::vec3 position, const int width, const int height, const float near, const float far);
        virtual ~PointLight();

        void settingShader(Shader *_shader);

        glm::mat4 getProjectionMatrix();
        std::vector<glm::mat4> getTransformMatrix();

    protected:
        int width;
        int height;

    private:
};

#endif // LIGHT_HPP
