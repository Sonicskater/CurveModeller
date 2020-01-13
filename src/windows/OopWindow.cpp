//
// Created by devon on 11/30/2019.
//
#pragma once
#include <cstdlib>
#include <iostream>
#include <program.hpp>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "mat4f.hpp"
#include "common_matrices.hpp"
#include "shader_file_io.hpp"

class OopWindow{
public:
    explicit OopWindow(const char *t_title, int width, int height): g_width(width), g_height(height), title(t_title) {
        if(!glfwInit()){
            exit(EXIT_FAILURE);
        }

        // set opengl version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        window = glfwCreateWindow(g_width,           // width
                                  g_height,           // height
                                  title, // title
                                  nullptr,           // unused
                                  nullptr);

        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glfwSwapInterval(1); // vsync
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSetWindowUserPointer(window, this);
        // setup callbacks

        // this kind of code allows for C++ code to be written properly with objects and still interop with glfw properly.
        glfwSetFramebufferSizeCallback(window, +[](GLFWwindow* win, int width, int height){
            static_cast<OopWindow*>(glfwGetWindowUserPointer(win)) -> setFrameBufferSize(width, height);
        });
    }

    void draw(){
        glfwMakeContextCurrent(window);

        // provide entry point for child classes to extend the draw loop.
        private_draw();

        glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
        glfwPollEvents();        // will process event queue and carry on

    }

    bool shouldClose(){
        return glfwWindowShouldClose(window);
    }

    void destroy(){
        glfwDestroyWindow(window);
    }

    virtual void private_draw(){};

private:
    int g_width, g_height;
    const char *title;

    void setFrameBufferSize(int width,int height){
        glfwMakeContextCurrent(window);
        g_width = width;
        g_height = height;
        glViewport(0, 0, g_width, g_height);
        g_P = math::perspectiveProjection(30, float(g_width) / float(g_height), 0.01, 100.f);
        std::cout << "Resized: " << title << "\n";
    }


protected:
    GLFWwindow* window = nullptr;
    math::Mat4f g_P = math::Mat4f::identity() , g_V = math::Mat4f::identity(), g_M = math::Mat4f::identity();

    // user defined alias
    opengl::Program createShaderProgram(std::string const &vertexShaderFile,
                                        std::string const &fragmentShaderFile) {
        glfwMakeContextCurrent(window);
        using namespace opengl;
        auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
        auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

        std::cout << "[Log] compiling program " << vertexShaderFile << ' '
                  << fragmentShaderFile << '\n';
        return opengl::makeProgram(vertexShaderSource, fragmentShaderSource);
    }
    static void setupVAO(GLuint vaoID, GLuint vboID) {

        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // set up position input into vertex shader
        glEnableVertexAttribArray(0);          // match layout # in shader
        glVertexAttribPointer(                 //
                0,                                 // attribute layout # (in shader)
                3,                                 // number of coordinates per vertex
                GL_FLOAT,                          // type
                GL_FALSE,                          // normalized?
                sizeof(math::Vec3f),                    // stride
                nullptr							  // array buffer offset
        );

        glBindVertexArray(0);
    }

    static bool loadGeometryToGPU(std::vector<math::Vec3f> const &vertices, GLuint vboID) {
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(
                GL_ARRAY_BUFFER,                  // destination
                sizeof(math::Vec3f) * vertices.size(), // size (bytes) of memcopy to GPU
                vertices.data(),                  // pointer to data (contiguous)
                GL_STATIC_DRAW                    // usage patter of the GPU buffer
        );

        return true;
    }

};

