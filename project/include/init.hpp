#ifndef INIT_HPP
#define INIT_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

void changeFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void initOpenGL(const int width, const int height);

#endif // INIT_HPP
