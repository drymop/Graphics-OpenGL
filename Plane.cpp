#include "Plane.h"

Plane::
Plane(glm::vec3 _point, glm::vec3 _normal)
  : m_point(_point), m_normal(glm::normalize(_normal)) {};

float
Plane::
intersectRay(Ray _ray) {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();

  float denom = glm::dot(d, m_normal);
  if (denom == 0) {
    return 0;
  }
  return glm::dot(m_point - p, m_normal) / denom;
}
