#ifndef SPHERE_H_
#define SPHERE_H_

#include "Material.h"
#include "RenderableObject.h"

class Sphere : public RenderableObject
{
  public:
    Sphere(glm::vec3 _center, float _radius, glm::vec3 _color);

    RayHit intersectRay(Ray _ray);

    glm::vec3 calculateColor();

  private:
    /// Center of the sphere
    glm::vec3 m_center;
    /// Radius of the sphere. Must be a positive number.
    float m_radius;
    /// Color of the sphere
    glm::vec3 m_color;
};

#endif // SPHERE_H_