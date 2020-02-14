#ifndef RASTERIZABLE_OBJECT_H_
#define RASTERIZABLE_OBJECT_H_

#include <string>

// GLM
#include <glm/glm.hpp>

#include "Material.h"
#include "ObjFileParser.h"


class RasterizableObject
{
  public:
    RasterizableObject(const std::string& _meshFile, 
                       const std::string& _materialFile, 
                       glm::mat4 _modelMatrix)
      : m_mesh(parseObjFile(_meshFile)), 
        m_material(parseMaterialFile(_materialFile)),
        m_modelMatrix(_modelMatrix) {};

  private:
    Mesh m_mesh;
    Material m_material;
    glm::mat4 m_modelMatrix;
};

#endif // RASTERIZABLE_OBJECT_H_