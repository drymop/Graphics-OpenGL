#ifndef PLANE_H_
#define PLANE_H_

#include "RayTracableObject.h"

class Plane : public RayTracableObject
{
  public:
    Plane(glm::vec3 _point, glm::vec3 _normal, Material _material)
      : m_point(_point), 
        m_normal(glm::normalize(_normal)),
        m_material(_material)
    {};

    glm::vec3 getNormal() const {
      return m_normal;
    }

    Material getMaterial() const {
      return m_material;
    }
    
    RayHit intersectRay(Ray _ray) const;
  
  private:
    /// A point wihin the plane
    glm::vec3 m_point;
    /// Unit vector specifying the normal of the plane
    glm::vec3 m_normal;
    /// Material on the surface of this plane
    struct Material m_material;
};

#endif // PLANE_H_