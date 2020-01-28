#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>

#include "Material.h"
#include "Ray.h"
#include "RenderableObject.h"

class Sphere : public RenderableObject
{
  public:
    Sphere(glm::vec3 _center, float _radius);

    float intersectRay(Ray _ray);

  private:
    /// Center of the sphere
    glm::vec3 m_center;
    /// Radius of the sphere. Must be a positive number.
    float m_radius;
};

#endif // SPHERE_H_