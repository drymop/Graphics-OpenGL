#ifndef PLANE_H_
#define PLANE_H_

#include "RenderableObject.h"

class Plane : public RenderableObject
{
  public:
    Plane(glm::vec3 _point, glm::vec3 _normal, glm::vec3 _color);

    RayHit intersectRay(Ray _ray);
  
    glm::vec3 calculateColor();
  
  private:
    /// A point wihin the plane
    glm::vec3 m_point;
    /// Unit vector specifying the normal of the plane
    glm::vec3 m_normal;
    /// Color of the plane
    glm::vec3 m_color;
};

#endif // PLANE_H_