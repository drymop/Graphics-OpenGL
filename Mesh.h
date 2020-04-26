#ifndef MESH_H_
#define MESH_H_

#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// @brief One possible storage of vertex information.
////////////////////////////////////////////////////////////////////////////////
struct Vertex {

  glm::vec3 p;  ///< Position
  glm::vec3 n;  ///< Normal
  glm::vec2 t;  ///< Texture
  glm::vec3 tg; ///< Tangent

  Vertex(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _t, const glm::vec3& _tg) :
    p(_p), n(_n), t(_t), tg(_tg) {}

  Vertex(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _t) :
    Vertex(_p, _n, _t, {0,0,0}) {}

  Vertex() : Vertex({0,0,0},{0,0,0},{0,0}) {};
};

////////////////////////////////////////////////////////////////////////////////
/// @brief One possible mesh data structure
///
/// Vertices will be ordered such that every three form a triangle, e.g.,
/// vertices at indices 0, 1, 2 form a triangle, and then vertices at indices
/// 3, 4, 5 form a triangle, etc.
////////////////////////////////////////////////////////////////////////////////
struct Mesh {
  std::vector<Vertex> vertices;

  Mesh(const std::vector<Vertex>& _vertices = std::vector<Vertex>()) : 
    vertices(_vertices) {};
};

#endif // MESH_H_