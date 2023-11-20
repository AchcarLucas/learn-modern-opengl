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
