#ifndef RASTERIZABLE_OBJECT_H_
#define RASTERIZABLE_OBJECT_H_

#include <string>

#include "GLInclude.h"
#include "ObjFileParser.h"
#include "RenderableObject.h"


/// The locations of uniforms related to material in rasterizer
struct MaterialUniformLocations {
  GLint ka;
  GLint kd;
  GLint ks;
  GLint shininess;
};

/// The locations of uniforms related the objects in rasterizer
struct ObjectUniformLocations {
  GLint vertexModelMatrix;
  GLint normalModelMatrix;
  MaterialUniformLocations material;
};

class RasterizableObject : public RenderableObject
{
  public:
    RasterizableObject(const Mesh& _mesh, 
                       const Material& _material, 
                       const glm::mat4& _modelMatrix);

    void setUniformLocations(const ObjectUniformLocations& _locs) {
      m_uniformLocations = _locs;
    };

    void draw();

  private:
    /// Number of vertices in the mesh
    size_t m_nVertices;
    /// Transformation of vertex from model to world
    glm::mat4 m_vModelMatrix;
    /// Transformation of normal from model to world
    glm::mat4 m_nModelMatrix;
    /// Name of vertex array object for this object
    GLuint m_vao;
    /// Location of uniform to send object data
    ObjectUniformLocations m_uniformLocations;
};

#endif // RASTERIZABLE_OBJECT_H_