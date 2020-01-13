//
// Created by devon on 11/30/2019.
//
#include <vector>
#include "program.hpp"
#include "OopWindow.cpp"
#include "vertex_array_object.hpp"
#include "common_matrices.hpp"
#include "buffer_object.hpp"



class CurveEditor : public OopWindow{
private:
    opengl::Program *program;
    opengl::Program basicShader = createShaderProgram("./shaders/basic_vs.glsl",
                                                      "./shaders/basic_fs.glsl");
    math::Vec3f color_curve = math::Vec3f(0, 1, 1);
    math::Vec3f color_control = math::Vec3f(1, 0, 0);
    std::vector<math::Vec3f> controlPoints;
    std::vector<math::Vec3f> outCurve;
    std::vector<math::Vec3f> selected;

    int item = 0;

    opengl::VertexArrayObject vao_control = opengl::makeVertexArrayObject(window),  vao_curve = opengl::makeVertexArrayObject(window), vao_select = opengl::makeVertexArrayObject(window);
    opengl::BufferObject vbo_control = opengl::makeBufferObject(window) , vbo_curve = opengl::makeBufferObject(window), vbo_select = opengl::makeBufferObject(window);

    static std::vector<math::Vec3f> Subdivide(const std::vector<math::Vec3f>& pred, int r){
        std::vector<math::Vec3f> out = std::vector<math::Vec3f>();
        out.emplace_back(pred[0]);

        for (int i = 1; i < (pred.size()-1); i++){
            out.push_back(pred[i-1]*0.25 + pred[i]*0.75);
            out.push_back(pred[i]*0.75 + pred[i+1]*0.25);
        }
        out.emplace_back(pred[pred.size()-1]);
        if(r) return Subdivide(out,r-1);
        return out;
    }
public:

    std::vector<math::Vec3f> out_curve;

    CurveEditor(const char *tTitle, int width, int height) : OopWindow(tTitle, width, height) {
        glfwMakeContextCurrent(window);

        math::Vec3f viewPosition(0, 0, 3);
        g_V = lookAtMatrix(viewPosition,    // eye position
                           { 0.f, 0.f, 0.f }, // look at
                           { 0.f, 1.f, 0.f }  // up vector
        );
        g_P = math::orthographicProjection(-1, 1, 1, -1, 0.001f, 10);

        setupVAO(vao_control.id(), vbo_control.id());
        setupVAO(vao_curve.id(), vbo_curve.id());
        setupVAO(vao_select.id(), vbo_select.id());

        //Load control points

        controlPoints.emplace_back( 0,0.5,0 );
        controlPoints.emplace_back( 0.5,0,0 );
        controlPoints.emplace_back( 0,-0.5,0 );

        // key callback
        glfwSetKeyCallback(window, +[](GLFWwindow* window, int key, int scancode, int action, int mods){
            auto me = static_cast<CurveEditor*>(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_D && action == GLFW_PRESS){
                me->item = (me->item + 1)% me->controlPoints.size();
            } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
                if (me->item == 0) {
                    me->item = me->controlPoints.size() - 1;
                } else {
                    me->item = (me->item - 1) % me->controlPoints.size();
                }
            } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
                me->controlPoints.at(me->item).operator-=(math::Vec3f(0.01,0,0));
            } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
                me->controlPoints.at(me->item).operator+=(math::Vec3f(0.01,0,0));
            } else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
                me->controlPoints.at(me->item).operator+=(math::Vec3f(0.0,0.01,0));
            } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                me->controlPoints.at(me->item).operator-=(math::Vec3f(0.00,0.01,0));
            } else if (key == GLFW_KEY_W && action == GLFW_PRESS){
                auto x = me->item;
                auto index = me->controlPoints.begin() + x + 1;
                if (me->controlPoints.size() > (x+1)) {
                    me->controlPoints.insert(index, (me->controlPoints.at(x)+me->controlPoints.at(x+1))*(0.5f));
                }

            } else if (key == GLFW_KEY_S && action == GLFW_PRESS){
                auto x = me->item;
                auto index = me->controlPoints.begin() + x;
                if  (x >= 1) {
                    me->controlPoints.insert(index, (me->controlPoints.at(x-1)+me->controlPoints.at(x))*(0.5f));
                }

            }else if (key == GLFW_KEY_X && action == GLFW_PRESS){
                auto x = me->item;
                if (me->controlPoints.size() > 2) {
                    me->controlPoints.erase(me->controlPoints.begin() + x);
                }
            }
        });

        //Set to one shader program
        program = &basicShader;
        glPointSize(10);
    }

    void private_draw() override {
        OopWindow::private_draw();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)


        loadGeometryToGPU(controlPoints, vbo_control.id());

        //Load curve points

        outCurve = Subdivide(controlPoints,2);	//TODO - insert subdivision here
        out_curve.clear();

        for (auto v : outCurve){
            out_curve.emplace_back(v + math::Vec3f(0.5,0,0));
        }
        loadGeometryToGPU(outCurve, vbo_curve.id());


        selected = {controlPoints.at(item)};
        loadGeometryToGPU(selected, vbo_select.id());

        program->use();
        opengl::setUniformMat4f(program->uniformLocation("model"), g_M, true);
        opengl::setUniformMat4f(program->uniformLocation("view"), g_V, true);
        opengl::setUniformMat4f(program->uniformLocation("projection"), g_P, true);

        //Draw curve
        opengl::setUniformVec3f(basicShader.uniformLocation("color"), color_curve);
        vao_curve.bind();
        /*
        glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
                     0,						  // offset into buffer
                     outCurve.size()	// number of vertices in buffer
        );
         */

        glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
                     0,						  // offset into buffer
                     outCurve.size()	// number of vertices in buffer
        );

        opengl::setUniformVec3f(basicShader.uniformLocation("color"), math::Vec3f(0,1,0));
        vao_select.bind();
        glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
                     0,						  // offset into buffer
                     selected.size()	// number of vertices in buffer
        );

        //Draw control points
        opengl::setUniformVec3f(basicShader.uniformLocation("color"), color_control);
        vao_control.bind();
        glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
                     0,						  // offset into buffer
                     controlPoints.size()	// number of vertices in buffer
        );

        glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
                     0,						  // offset into buffer
                     controlPoints.size()	// number of vertices in buffer
        );


    }
};
