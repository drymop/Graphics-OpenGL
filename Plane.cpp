#include "Plane.h"

RayHit
Plane::
intersectRay(Ray _ray) const {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();

  float denom = glm::dot(d, m_normal);
  RayHit hit;
  if (denom == 0) {
    return RayHit();
  }
  float t = glm::dot(m_point - p, m_normal) / denom;
  glm::vec3 hitPos = p + d * t;
  return {t, hitPos, m_normal, m_material};
}
