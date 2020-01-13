//
// Created by devon on 12/8/2019.
//

#ifndef CURVES_MESHVIEWER_H
#define CURVES_MESHVIEWER_H

#include <vertex_array_object.hpp>
#include <buffer_object.hpp>
#include <obj_mesh.hpp>
#include <imgui.h>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"


#include "OopWindow.cpp"

class MeshViewer : public OopWindow  {
private:
    opengl::VertexArrayObject vao = opengl::makeVertexArrayObject(window);
    opengl::BufferObject vertexBuffer = opengl::makeBufferObject(window);
    opengl::BufferObject indexBuffer = opengl::makeBufferObject(window);

    GLuint totalIndices = 0;

    glm::mat4 view;



    math::Vec3f lightPosition = math::Vec3f(1, 0, 1);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    ImGuiIO* io;

    float yaw = 0;
    float pitch = 0;
    float roll = 0;

    float x = 0;
    float y = 0;
    float z = 0;

    float lightx=1;
    float lighty = 1;
    float lightz=1;

    opengl::Program phongSolidShader;

    glm::mat4 transform = glm::mat4(1.0f);
public:

    std::vector<math::Vec3f> controlPoints;
    MeshViewer(const char* tTitle, int width, int height);

    void private_draw() override;
};


#endif //CURVES_MESHVIEWER_H
