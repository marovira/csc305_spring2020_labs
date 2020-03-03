#pragma once

#include "paths.hpp"

#include <exception>
#include <iostream>
#include <string>

#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>

#include <fmt/printf.h>
#include <magic_enum.hpp>

using namespace atlas;

static const std::vector<std::string> IncludeDir{ShaderPath};

struct OpenGLError : std::runtime_error
{
    OpenGLError(const std::string& what_arg) : std::runtime_error(what_arg){};
    OpenGLError(const char* what_arg) : std::runtime_error(what_arg){};
};

class Triangle
{
public:
    Triangle();

    void loadShaders();

    void loadDataToGPU(std::array<float, 18> const& vertices);

    void reloadShaders();

    void render();

    void freeGPUData();

private:
    // Vertex buffers.
    GLuint mVao;
    GLuint mVbo;

    // Shader data.
    GLuint mVertHandle;
    GLuint mFragHandle;
    GLuint mProgramHandle;
    glx::ShaderFile vertexSource;
    glx::ShaderFile fragmentSource;
};

class Program
{
public:
    Program(int width, int height, std::string title);

    void run(Triangle& triangles);

    void freeGPUData();

private:
    static void errorCallback(int code, char const* message)
    {
        fmt::print("error ({}): {}\n", code, message);
    }

    void createGLContext();

    GLFWwindow* mWindow;
    glx::WindowSettings settings;
    glx::WindowCallbacks callbacks;
};
