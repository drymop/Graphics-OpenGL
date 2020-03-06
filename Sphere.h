#ifndef SPHERE_H_
#define SPHERE_H_

#include "RayTracableObject.h"

class Sphere : public RayTracableObject
{
  public:
    Sphere(glm::vec3 _center, float _radius, struct Material _material) 
      : RayTracableObject(_material),
        m_center(_center), 
        m_radius(_radius)
    {};

    RayHit intersectRay(Ray _ray) const;

  private:
    /// Center of the sphere
    glm::vec3 m_center;
    /// Radius of the sphere. Must be a positive number.
    float m_radius;
};

#endif // SPHERE_H_