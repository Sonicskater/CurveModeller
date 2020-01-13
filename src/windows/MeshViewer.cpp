//
// Created by devon on 12/8/2019.
//

#include "MeshViewer.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vbo_tools.hpp>
#include "vec3f.hpp"

MeshViewer::MeshViewer(const char *tTitle, int width, int height) : OopWindow(tTitle, width, height), phongSolidShader(
        createShaderProgram("./shaders/phong_vs.glsl",
                            "./shaders/phong_fs.glsl")
        ) {
    glfwMakeContextCurrent(window);

    phongSolidShader.use();
    opengl::setUniformVec3f(phongSolidShader.uniformLocation("lightPos"), lightPosition);
    opengl::setUniformVec3f(phongSolidShader.uniformLocation("lookPos"), math::Vec3f(0, 0, 3));
    opengl::setUniform1f(phongSolidShader.uniformLocation("ambient"), 0.1);

    view = glm::lookAt(
            glm::vec3(0, 0, 3),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;



    //controlPoints.emplace_back( 1,1,0.1 );



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    phongSolidShader.use();

    projection = glm::perspective(glm::radians(90.0f), 800.f / 800.f, 0.1f, 100.0f);
}

void MeshViewer::private_draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    OopWindow::private_draw();

    ImGui::StyleColorsDark();
    //imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    ImGui::SliderFloat("Yaw", &yaw, -3.2f, 3.2f);
    ImGui::SliderFloat("Pitch", &pitch, -3.2f, 3.2f);
    ImGui::SliderFloat("Roll", &roll, -3.2f, 3.2f);

    ImGui::SliderFloat("X", &x, -5.f, 5.f);
    ImGui::SliderFloat("Y", &y, -5.f, 5.f);
    ImGui::SliderFloat("Z", &z, -5.f, 5.f);

    ImGui::Text("test");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::SliderFloat("Light X", &lightx, -5.f, 5.f);
    ImGui::SliderFloat("Light Y", &lighty, -5.f, 5.f);
    ImGui::SliderFloat("Light Z", &lightz, -5.f, 5.f);

    ImGui::End();

    int size = controlPoints.size();

    std::vector<math::Vec3f> vertices;
    int count = 32;
    for (int i = 0; i <= count; ++i){
        float angle = (i/ (float)count) * 3.14158f * 2.0f;
        float q = std::cos(angle);
        float j = std::sin(angle);
        for (auto w : controlPoints) {

            math::Vec3f v = math::Vec3f(w.x,w.y,w.z);

            math::Vec3f k = math::Vec3f(v.x*q,v.y,v.x*j);
            //std::cout << k << "\n";
            vertices.emplace_back(k);
        }
    }
    geometry::OBJMesh meshData;

    meshData.vertices = vertices;

    for (int i = 1; i <= count; ++i) {
        for (int j = 1; j < size; ++j) {

            int LL = (i-1)*size + j-1;
            int UL = (i%count)*size + j-1;
            int UR = (i%count)*size + j;
            int LR = (i-1)*size + j;

            auto a = geometry::Indices();
            a.vertexID() = LL;
            auto b = geometry::Indices();
            b.vertexID() = UR;
            auto c = geometry::Indices();
            c.vertexID() = UL;

            auto tri = geometry::Triangle_<geometry::Indices>({a, b, c});

            meshData.triangles.emplace_back(tri);

            auto a1 = geometry::Indices();
            a1.vertexID() = LL;
            auto b1 = geometry::Indices();
            b1.vertexID() = LR;
            auto c1 = geometry::Indices();
            c1.vertexID() = UR;

            auto tri1 = geometry::Triangle_<geometry::Indices>({a1, b1, c1});

            meshData.triangles.emplace_back(tri1);
        }
    }

    geometry::Normals normals = geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);

    opengl::VBOData_VerticesNormals vboData = opengl::makeConsistentVertexNormalIndices(meshData, normals);

    totalIndices = opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);

    vao.bind();

    projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.0f);

    transform = glm::mat4(1.0f);

    transform = glm::translate(transform, glm::vec3(x, y, z));

    transform = glm::rotate(transform, yaw, glm::vec3(0,1,0));
    transform = glm::rotate(transform, roll, glm::vec3(0, 0, 1));
    transform = glm::rotate(transform, pitch, glm::vec3(1, 0, 0));

    lightPosition = math::Vec3f(lightx, lighty, lightz);

    phongSolidShader.use();
    opengl::setUniformVec3f(phongSolidShader.uniformLocation("lightPos"), lightPosition);
    opengl::setUniformMat4f(phongSolidShader.uniformLocation("model"), transform, false);
    opengl::setUniformMat4f(phongSolidShader.uniformLocation("view"), view, false);
    opengl::setUniformMat4f(phongSolidShader.uniformLocation("projection"), projection, false);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawElements(GL_TRIANGLES,
                   totalIndices,    // # of triangles * 3
                   GL_UNSIGNED_INT, // type of indices
                   (void*)nullptr        // offset
    );

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

