#include "light.hpp"

Light::Light(glm::vec3 position)
{
    this->position = position;
}

Light::~Light()
{
    //dtor
}

DirectionalLight::DirectionalLight(const glm::vec3 position, const glm::vec3 direction, const float box_size, const float near, const float far) : Light(position)
{
    this->near_plane = near;
    this->far_plane = far;

    this->box_size = box_size;

    this->direction = direction;
}

DirectionalLight::~DirectionalLight()
{
    //dtor
}

glm::mat4 DirectionalLight::getViewMatrix()
{
    return glm::lookAt(this->position, this->direction, this->up);;
}

glm::mat4 DirectionalLight::getProjectionMatrix()
{
    return glm::ortho(-1 * this->box_size, this->box_size, -1 * this->box_size, this->box_size, this->near_plane, this->far_plane);;
}

PointLight::PointLight(const glm::vec3 position, const int width, const int height, const float near, const float far) : Light(position)
{
    this->near_plane = near;
    this->far_plane = far;
    this->width = width;
    this->height = width;
}

PointLight::~PointLight()
{
    //dtor
}

glm::mat4 PointLight::getProjectionMatrix()
{
    return glm::perspective(glm::radians(90.0f), (float)width/(float)height, near_plane, far_plane);
}

std::vector<glm::mat4> PointLight::getTransformMatrix()
{
    std::vector<glm::mat4> light_transform;

    glm::mat4 proj = getProjectionMatrix();

    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    light_transform.push_back(proj * glm::lookAt(position, position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

    return light_transform;
}
