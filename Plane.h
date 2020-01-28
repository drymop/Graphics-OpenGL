#ifndef PLANE_H_
#define PLANE_H_

#include <glm/glm.hpp>

#include "RenderableObject.h"

class Plane : public RenderableObject
{
  public:
    Plane(glm::vec3 _point, glm::vec3 _normal);

    float intersectRay(Ray _ray);
  
  private:
    /// A point wihin the plane
    glm::vec3 m_point;
    /// Unit vector specifying the normal of the plane
    glm::vec3 m_normal;  
};

#endif // PLANE_H_