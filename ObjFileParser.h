#ifndef OBJ_FILE_PARSER_H_
#define OBJ_FILE_PARSER_H_

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Material.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an obj file into a mesh
/// @param _filename Filename
/// @return Loaded mesh.
Mesh parseObjFile(const std::string& _filename);

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an mtl file to a material
/// @param _filename Filename
/// @return Loaded material
MaterialConfig parseMaterialFile(const std::string& _filename);


#endif // OBJ_FILE_PARSER_H_