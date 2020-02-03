#ifndef PLANE_H_
#define PLANE_H_

#include "RenderableObject.h"

class Plane : public RenderableObject
{
  public:
    Plane(glm::vec3 _point, glm::vec3 _normal, struct Material _material);
    
    RayHit intersectRay(Ray _ray);
  
    struct Material getMaterial();
  
  private:
    /// A point wihin the plane
    glm::vec3 m_point;
    /// Unit vector specifying the normal of the plane
    glm::vec3 m_normal;
    /// Material on the surface of this plane
    struct Material m_material;
};

#endif // PLANE_H_