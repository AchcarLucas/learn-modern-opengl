#ifndef INIT_HPP
#define INIT_HPP

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

void changeFrameBufferSizeCallback(GLFWwindow*, int, int);
void initOpenGL(const int, const int);

#endif // INIT_HPP
