#ifndef RASTERIZABLE_OBJECT_H_
#define RASTERIZABLE_OBJECT_H_

#include <string>

// GL
#define GL_GLEXT_PROTOTYPES
#if   defined(OSX)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#elif defined(LINUX)
#include <GL/glut.h>
#endif
#include <GL/freeglut.h>

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "Material.h"
#include "ObjFileParser.h"


/// The locations of uniforms related to material in shader
struct MaterialUniformLocation {
  GLint kaLoc;
  GLint kdLoc;
  GLint ksLoc;
  GLint shininessLoc;
};

class RasterizableObject
{
  public:
    RasterizableObject(const Mesh& _mesh, 
                       const Material& _material, 
                       const glm::mat4& _vModelMatrix,
                       const glm::mat4& _nModelMatrix);

    void setMaterialUniformLocation(const MaterialUniformLocation& _loc) {
      m_materialUniformLocation = _loc;
    }

    void setVertexModelMatrixUniformLocation(GLint _loc) {
      m_vModelMatrixLocation = _loc;
    }

    void setNormalModelMatrixUniformLocation(GLint _loc) {
      m_nModelMatrixLocation = _loc;
    }

    void draw();

  private:
    /// Number of vertices in the mesh
    size_t m_nVertices;
    /// Material of the object
    Material m_material;
    /// Transformation of vertex from model to world
    glm::mat4 m_vModelMatrix;
    /// Transformation of normal from model to world
    glm::mat4 m_nModelMatrix;
    /// Name of vertex array object for this object
    GLuint m_vao;
    /// Location of uniform to store object material
    MaterialUniformLocation m_materialUniformLocation;
    /// Location of uniform to store object transform
    GLint m_vModelMatrixLocation;
    GLint m_nModelMatrixLocation;
};

#if   defined(OSX)
#pragma clang diagnostic pop
#endif

#endif // RASTERIZABLE_OBJECT_H_