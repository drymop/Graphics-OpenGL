#include "Plane.h"

Plane::
Plane(glm::vec3 _point, glm::vec3 _normal, struct Material _material)
  : m_point(_point), 
    m_normal(glm::normalize(_normal)),
    m_material(_material)
{};

RayHit
Plane::
intersectRay(Ray _ray) {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();

  float denom = glm::dot(d, m_normal);
  RayHit hit;
  if (denom == 0) {
    return RayHit();
  }
  float t = glm::dot(m_point - p, m_normal) / denom;
  glm::vec3 hitPos = p + d * t;
  return {t, hitPos, m_normal};
}

struct Material
Plane::
getMaterial() {
  return m_material;
}
