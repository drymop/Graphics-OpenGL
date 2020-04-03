#ifndef PLANE_H_
#define PLANE_H_

#include "RayTracableObject.h"

class Plane : public RayTracableObject
{
  public:
    Plane(
      glm::vec3 _point, 
      glm::vec3 _normal, 
      const MaterialConfig& _materialConfig,
      bool _generateMesh = false)
      : RayTracableObject(_materialConfig),
        m_point(_point), 
        m_normal(glm::normalize(_normal))
    {};

    glm::vec3 getNormal() const {
      return m_normal;
    }
    
    RayHit intersectRay(Ray _ray) const;
  
  private:
    /// A point wihin the plane
    glm::vec3 m_point;
    /// Unit vector specifying the normal of the plane
    glm::vec3 m_normal;
};

#endif // PLANE_H_