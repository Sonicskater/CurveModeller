cmake_minimum_required(VERSION 3.0)
project(Curves VERSION 1.0 LANGUAGES C CXX)

#[[
        OpenGL
]]
find_package(OpenGL REQUIRED)

# 3rd party libraries
include(cmake/external.cmake)


#[[
	stb image
]]
set(STB_DIR
	external/stb
	)




#[[
        Headers
]]
set(HEADERS
   include/vec3f.hpp
   include/vec2f.hpp
   include/mat3f.hpp
   include/mat4f.hpp
   include/common_matrices.hpp
   include/triangle.hpp
   include/triangle.tpp
   include/obj_mesh.hpp
   include/obj_mesh_file_io.hpp
   include/shader.hpp
   include/shader_file_io.hpp
   include/vertex_array_object.hpp
   include/program.hpp
   include/buffer_object.hpp
   include/object.hpp
   include/vbo_tools.hpp
   include/texture.hpp
   include/image.hpp
   )

#[[
        Sources
]]
set(SOURCES
    src/main.cpp
    src/vec3f.cpp
    src/vec2f.cpp
    src/mat4f.cpp
    src/mat3f.cpp
    src/common_matrices.cpp
    src/triangle.cpp
    src/obj_mesh.cpp
    src/obj_mesh_file_io.cpp
    src/shader.cpp
    src/shader_file_io.cpp
    src/vertex_array_object.cpp
    src/program.cpp
    src/buffer_object.cpp
    src/object.cpp
    src/vbo_tools.cpp
    src/texture.cpp
    src/image.cpp
		src/windows/OopWindow.cpp
		src/windows/CurveEditor.cpp
		src/windows/MeshViewer.cpp
		src/windows/MeshViewer.h)

#[[
        GLM
]]
set(GLM_DIR external/glm)
set(GLM_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLM example programs")
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "Build the GLM test programs")
set(GLM_BUILD_DOCS OFF CACHE INTERNAL "Build the GLM documentation")
set(GLM_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLM_DIR})

#[[
	Shaders
]]
set(SHADERS
	shaders/basic_vs.glsl
	shaders/basic_fs.glsl
		shaders/phong_vs.glsl
		shaders/phong_fs.glsl
	)

foreach(file ${SHADERS})
	configure_file(${file} ${file}  COPYONLY)
endforeach(file)
	
#[[
	Resources
]]
set(RESOURCES
	${SHADERS}
	)

#[[
        Executable
]]
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PRIVATE external
    PRIVATE ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
	PRIVATE ${STB_DIR}
	${IMGUI_INCLUDE_DIR}
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE -DGLFW_INCLUDE_NONE
    )

if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -D_USE_MATH_DEFINES
		PRIVATE -DNOMINMAX
        )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER org.cmake.${PROJECT_NAME}
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE GLFW_INCLUDE_NONE
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OPENGL_gl_LIBRARY}
    PRIVATE glfw
    PRIVATE ${GLFW_LIBRARIES}
    PRIVATE glad
    PRIVATE ${GLAD_LIBRARIES}
    PRIVATE ${CMAKE_DL_LIBS}
	PRIVATE imgui
	PRIVATE ${IMGUI_LIBRARIES}
    )



