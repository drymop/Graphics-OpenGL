#include "RasterizableObject.h"

#include <limits>

// test
#include <iostream>

using glm::cross, glm::dot, glm::value_ptr, glm::vec3, glm::vec4;

RasterizableObject::
RasterizableObject(const Mesh& _mesh, 
                   const Material& _material, 
                   const glm::mat4& _modelMatrix)
  : RayTracableObject(_material),
    m_mesh(_mesh),
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
                        sizeof(Vertex), (void*)sizeof(vec3));
  // - textures
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*)(sizeof(vec3)+sizeof(vec3)));

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // mesh stuff
  for(auto& v : m_mesh.vertices) {
    v.p = vec3(m_vModelMatrix * vec4(v.p, 1));
    v.n = glm::normalize(vec3(m_nModelMatrix * vec4(v.n, 0)));
  }
}


void
RasterizableObject::
draw() {
  // set tranformation uniforms
  glUniformMatrix4fv(m_uniformLocations.vertexModelMatrix, 
                     1, GL_FALSE, value_ptr(m_vModelMatrix));
  glUniformMatrix4fv(m_uniformLocations.normalModelMatrix, 
                     1, GL_FALSE, value_ptr(m_nModelMatrix));
  // set texture uniforms
  glUniform1i(m_uniformLocations.hasKdMap, false);
  glUniform1i(m_uniformLocations.hasKsMap, false);
  // set default material uniforms
  glUniform3fv(m_uniformLocations.material.ka, 1, value_ptr(m_material.ka));
  glUniform3fv(m_uniformLocations.material.kd, 1, value_ptr(m_material.kd));
  glUniform3fv(m_uniformLocations.material.ks, 1, value_ptr(m_material.ks));
  glUniform1f (m_uniformLocations.material.shininess, m_material.shininess);
  // draw
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
  glBindVertexArray(0);
}

const float RasterizableObject::SELF_INTERSECTION_BIAS = 1e-3f;

RayHit
RasterizableObject::
intersectRay(Ray _ray) const {
  RayHit hitResult;
  bool isHit = false;
  hitResult.t = std::numeric_limits<float>::infinity();
  for (int i = 0; i < m_nVertices; i+=3) {
    isHit = isHit || intersectRayTriangle(
      _ray,
      m_mesh.vertices[i], m_mesh.vertices[i+1], m_mesh.vertices[i+2], 
      &hitResult);
  }
  if (!isHit) return RayHit();
  // convert hit result back to world coordinate
  return hitResult;
}

bool
RasterizableObject::
intersectRayTriangle(
    Ray ray,
    const Vertex& v0, 
    const Vertex& v1, 
    const Vertex& v2, 
    RayHit* hitResult) const {
  // use Möller-Trumbore algorithm to find barycentric coordinates 
  // (b, c) of the intersection
  // and distance t from ray origin to ray 

  vec3 rayOrigin = ray.getOrigin();
  vec3 rayDir = ray.getDirection();
  // edges of the triangle
  vec3 e1 = v1.p - v0.p;
  vec3 e2 = v2.p - v0.p;

  vec3 pVec = cross(rayDir, e2);
  float det = dot(e1, pVec);

  // det < 0: triangle faces away
  // det == 0: triangle parallel to ray
  // det > 0: triangle faces the ray
  if (det == 0) {
    return false;
  }
  float invDet = 1 / det;
  vec3 tVec = rayOrigin - v0.p;

  // first barycentric coordinate
  float b = dot(tVec, pVec) * invDet;
  if (b < 0 || b > 1) return false;

  vec3 qVec = cross(tVec, e1);
  // second barycentric coordinate
  float c = dot(rayDir, qVec) * invDet;
  if (c < 0 || b + c > 1) return false;
  // hit time
  float t = dot(e2, qVec) * invDet;
  if (t <= SELF_INTERSECTION_BIAS || hitResult->t <= t) return false;
  float a = 1 - b - c;

  // actual hit, compute hit result
  hitResult->t = t;
  hitResult->position = rayOrigin + rayDir*t;
  hitResult->normal = a*v0.n + b*v1.n + c*v2.n;
  hitResult->material = m_material;
  return true;
}