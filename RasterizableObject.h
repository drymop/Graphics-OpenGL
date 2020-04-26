#ifndef RASTERIZABLE_OBJECT_H_
#define RASTERIZABLE_OBJECT_H_

#include <string>

#include "GLInclude.h"
#include "Mesh.h"
#include "RayTracableObject.h"


/// The locations of uniforms related to material in rasterizer
struct MaterialUniformLocations {
  GLint ka;
  GLint kd;
  GLint ks;
  GLint ke;
  GLint shininess;
};

/// The locations of uniforms related the objects in rasterizer
struct ObjectUniformLocations {
  GLint vertexModelMatrix;
  GLint normalModelMatrix;
  GLint hasKdMap;
  GLint hasKsMap;
  GLint hasKeMap;
  GLint hasNormalMap;
  GLint hasParallaxMap;
  MaterialUniformLocations material;
};

class RasterizableObject : public RayTracableObject
{
  public:
    RasterizableObject(const Mesh& _mesh, 
                       const MaterialConfig& _materialConfig,
                       const glm::mat4& _modelMatrix);

    virtual void setUniformLocations(const ObjectUniformLocations& _locs) {
      m_uniformLocations = _locs;
    };

    virtual void sendMeshData();

    virtual void draw();

    /// Ray hit with t not exceeding this amount is treated as
    /// an object hitting itself, and thus doesn't count as hitting
    /// another object
    static const float SELF_INTERSECTION_BIAS;

    RayHit intersectRay(Ray _ray) const override;

  protected:
    Mesh m_mesh;
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

    ////////////////////////////////////////////////////////////////////////////
    // Set the object uniforms before draw call
    void sendUniformData();

    ////////////////////////////////////////////////////////////////////////////
    // Transform the vertex to world coordinate
    Vertex vertexToWorld(const Vertex& v) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Check if ray intersect a triangle, and that the intersection is closer 
    /// to the ray origin. Write hit result into hitResult param.
    /// @return whether the ray intersects the triangle closer to the current hit
    bool intersectRayTriangle(Ray ray,
                              const Vertex& v0, 
                              const Vertex& v1, 
                              const Vertex& v2, 
                              RayHit* hitResult) const;
};

#endif // RASTERIZABLE_OBJECT_H_