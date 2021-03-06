/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#pragma once

#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>

#include "object.hpp"

namespace opengl {

class BufferObject final {
public:
  enum Type : GLenum {
    INVALID = GL_INVALID_ENUM,
    ARRAY = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
    TEXTURE = GL_TEXTURE_BUFFER
  };

public:
  BufferObject::Type type() const;
  bool isAllocated() const;
  bool isValidType() const;
  bool isValid() const;

  explicit operator bool() const;

  void bind() const;
  void bind(Type type);

  void unbind();

  GLuint id() const;

private:
  /* Only called through makeBufferObject() factory function */
  explicit BufferObject(GLuint id);
  void release();


    friend BufferObject makeBufferObject();
    friend BufferObject makeBufferObject(GLFWwindow* context);
  friend void swap(BufferObject &lhs, BufferObject &rhs);

private:
  opengl::Object<BufferObject> m_id;
  BufferObject::Type m_type = INVALID;
};

BufferObject makeBufferObject();
BufferObject makeBufferObject(GLFWwindow* context);
void swap(BufferObject &lhs, BufferObject &rhs);

bool isValidBufferObjectType(BufferObject::Type type);

BufferObject::Type enumToBufferObjectType(GLenum bufferObjectEnum);

} // namespace openGL
