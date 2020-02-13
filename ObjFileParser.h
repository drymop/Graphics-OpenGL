#ifndef OBJ_FILE_PARSER_H_
#define OBJ_FILE_PARSER_H_

#include <vector>
#include <string>

#include <glm/glm.hpp>


////////////////////////////////////////////////////////////////////////////////
/// @brief One possible storage of vertex information.
////////////////////////////////////////////////////////////////////////////////
struct Vertex {

  glm::vec3 m_p; ///< Position
  glm::vec3 m_n; ///< Normal
  glm::vec2 m_t; ///< Texture

  Vertex(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _t) :
    m_p(_p), m_n(_n), m_t(_t) {}
};

////////////////////////////////////////////////////////////////////////////////
/// @brief One possible mesh data structure
///
/// Vertices will be ordered such that every three form a triangle, e.g.,
/// vertices at indices 0, 1, 2 form a triangle, and then vertices at indices
/// 3, 4, 5 form a triangle, etc.
////////////////////////////////////////////////////////////////////////////////
struct Mesh {
  std::vector<Vertex> m_vertices;

  Mesh(const std::vector<Vertex>& _vertices = std::vector<Vertex>()) : 
    m_vertices(_vertices) {}
};

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an obj file into a mesh
/// @param _filename Filename
/// @return Loaded mesh. If error, an empty mesh will be returned
Mesh parseObjFile(const std::string& _filename);


#endif // OBJ_FILE_PARSER_H_