#include "RasterizableObject.h"


RasterizableObject::
RasterizableObject(const Mesh& _mesh, 
                   const Material& _material, 
                   const glm::mat4& _modelMatrix)
  : RenderableObject(_material),
    m_nVertices(_mesh.vertices.size()),
    m_vModelMatrix(_modelMatrix),
    m_nModelMatrix(glm::transpose(glm::inverse(_modelMatrix)))
{
  // Create vertex array object
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // Create buffer to store mesh data
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(Vertex) * m_nVertices, 
               &_mesh.vertices[0], 
               GL_STATIC_DRAW);
  // Specify vertex attributes within buffer (interleave)
  // - positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), 0);
  // - normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*)sizeof(glm::vec3));

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void
RasterizableObject::
draw() {
  GLint vertexModelMatrix;
  GLint normalModelMatrix;
  // set tranformation uniforms
  glUniformMatrix4fv(m_uniformLocations.vertexModelMatrix, 
                     1, GL_FALSE, glm::value_ptr(m_vModelMatrix));
  glUniformMatrix4fv(m_uniformLocations.normalModelMatrix, 
                     1, GL_FALSE, glm::value_ptr(m_nModelMatrix));
  // set material uniforms
  glUniform3fv(m_uniformLocations.material.ka, 1, glm::value_ptr(m_material.ka));
  glUniform3fv(m_uniformLocations.material.kd, 1, glm::value_ptr(m_material.kd));
  glUniform3fv(m_uniformLocations.material.ks, 1, glm::value_ptr(m_material.ks));
  glUniform1f (m_uniformLocations.material.shininess, m_material.shininess);
  // draw
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
  glBindVertexArray(0);
}