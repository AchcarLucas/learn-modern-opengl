#include "light.hpp"

Light::Light(glm::vec3 position, const float near_plane, const float far_plane)
{
    this->position = position;
    this->near_plane = near_plane;
    this->far_plane = far_plane;
}

Light::~Light()
{
    //dtor
}

DirectionalLight::DirectionalLight(const glm::vec3 position, const glm::vec3 direction, const float box_size, const float near, const float far) : Light(position, near, far)
{
    this->box_size = box_size;
    this->direction = direction;
}

DirectionalLight::~DirectionalLight()
{
    //dtor
}

void DirectionalLight::settingShader(Shader *_shader)
{
    _shader->use();

    _shader->setUniform3fv("light.position", glm::value_ptr(this->getPosition()));

    _shader->setMatrix4fv("light.view", glm::value_ptr(this->getLightSpaceMatrix()));

    _shader->setFloat("light.near_plane", this->getNearPlane());
    _shader->setFloat("light.far_plane", this->getFarPlane());
}

glm::mat4 DirectionalLight::getViewMatrix()
{
    return glm::lookAt(this->position, this->direction, this->up);;
}

glm::mat4 DirectionalLight::getProjectionMatrix()
{
    return glm::ortho(-1 * this->box_size, this->box_size, -1 * this->box_size, this->box_size, this->near_plane, this->far_plane);;
}

PointLight::PointLight(const glm::vec3 position, const int width, const int height, const float near, const float far) : Light(position, near, far)
{
    this->width = width;
    this->height = width;
}

PointLight::~PointLight()
{
    //dtor
}

void PointLight::settingShader(Shader *_shader)
{
    _shader->use();

    _shader->setUniform3fv("light.position", glm::value_ptr(this->getPosition()));

    std::vector<glm::mat4> light_transform = this->getTransformMatrix();

    for(unsigned i = 0; i < light_transform.size(); ++i) {
        _shader->setMatrix4fv("light.views[" + std::to_string(i) + "]", glm::value_ptr(light_transform[i]));
    }

    _shader->setFloat("light.near_plane", this->getNearPlane());
    _shader->setFloat("light.far_plane", this->getFarPlane());
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
