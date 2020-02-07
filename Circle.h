#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Plane.h"

class Circle : public Plane
{
  public:
    Circle(glm::vec3 _center, float _radius, glm::vec3 _normal, Material _m)
      : Plane(_center, _normal, _m),
        m_center(_center),
        m_radius(_radius),
        m_radiusSq(_radius * _radius)
    {}

    glm::vec3 getCenter() const {
      return m_center;
    }

    float getRadius() const {
      return m_radius;
    }

    float getRadiusSq() const {
      return m_radiusSq;
    }

    RayHit intersectRay(Ray _ray) const override;

  private:
    /// Center of the circle
    glm::vec3 m_center;
    /// Radius of the circle
    float m_radius;
    /// Radius square of the circle
    float m_radiusSq;
};

#endif // CIRCLE_H_