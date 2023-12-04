#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Light
{
    public:
        Light(glm::vec3 position);
        virtual ~Light();

        glm::vec3 getPosition() { return position; }

    protected:
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 position;

    private:
};

class DirectionalLight : public Light
{
    public:
        DirectionalLight(const glm::vec3 position, const glm::vec3 direction, const float box_size, const float near, const float far);
        virtual ~DirectionalLight();

        void setPosition(glm::vec3 position) { this->position = position; }
        void setDirection(glm::vec3 direction) { this->direction = direction; }

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getLightSpaceMatrix() { return this->getProjectionMatrix() * this->getViewMatrix(); }

        float getNearPlane() { return this->near_plane; }
        float getFarPlane() { return this->far_plane; }
        float getBoxSize() { return this->box_size; }

    protected:
        float near_plane;
        float far_plane;
        float box_size;

        glm::vec3 direction;

    private:
};

class PointLight : public Light
{
    public:
        PointLight(const glm::vec3 position, const int width, const int height, const float near, const float far);
        virtual ~PointLight();

        void setPosition(glm::vec3 position) { this->position = position; }

        glm::mat4 getProjectionMatrix();
        std::vector<glm::mat4> getTransformMatrix();

        float getNearPlane() { return this->near_plane; }
        float getFarPlane() { return this->far_plane; }

    protected:
        int width;
        int height;

        float near_plane;
        float far_plane;

    private:
};

#endif // LIGHT_HPP
