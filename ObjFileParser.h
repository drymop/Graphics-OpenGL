#ifndef OBJ_FILE_PARSER_H_
#define OBJ_FILE_PARSER_H_

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Material.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief One possible storage of vertex information.
////////////////////////////////////////////////////////////////////////////////
struct Vertex {

  glm::vec3 p; ///< Position
  glm::vec3 n; ///< Normal

  Vertex(const glm::vec3& _p, const glm::vec3& _n) :
    p(_p), n(_n) {}
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

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an obj file into a mesh
/// @param _filename Filename
/// @return Loaded mesh.
Mesh parseObjFile(const std::string& _filename);

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an mtl file to a material
/// @param _filename Filename
/// @return Loaded material
Material parseMaterialFile(const std::string& _filename);


#endif // OBJ_FILE_PARSER_H_