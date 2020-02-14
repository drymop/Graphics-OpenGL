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


class RasterizableObject
{
  public:
    RasterizableObject(const Mesh& _mesh, 
                       const Material& _material, 
                       const glm::mat4& _modelMatrix);

    void draw();

  private:
    /// Name of vertex array object for this object
    GLuint m_vao;
    /// Number of vertices in the mesh
    size_t m_nVertices;
};

#if   defined(OSX)
#pragma clang diagnostic pop
#endif

#endif // RASTERIZABLE_OBJECT_H_