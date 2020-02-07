#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Plane.h"

class Circle : public Plane
{
  public:
    Circle(glm::vec3 _center, float _radius, glm::vec3 _normal, Material _m)
      : Plane(_center, _normal, _m),
        m_center(_center),
        m_radius_sq(_radius * _radius)
    {}

    RayHit intersectRay(Ray _ray) const override;

  private:
    /// Center of the circle
    glm::vec3 m_center;
    /// Radius square of the circle
    float m_radius_sq;
};

#endif // CIRCLE_H_