// STL
#include <cmath>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "Sphere.h"

Sphere::
Sphere(glm::vec3 _center, float _radius, struct Material _material) 
  : m_center(_center), 
    m_radius(_radius), 
    m_material(_material)
{};


RayHit 
Sphere::
intersectRay(Ray _ray) {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();
  glm::vec3 pMinusC = p - m_center;
  float bHalf = glm::dot(d, pMinusC);
  float c = glm::length2(pMinusC) - m_radius * m_radius;

  float discriminant = bHalf * bHalf - c;
  // no intersection
  if (discriminant <= 0) {
    return RayHit();
  }
  // return the smaller t
  float t = -bHalf - sqrt(discriminant);
  glm::vec3 hitPos = p + d * t;
  glm::vec3 normal = glm::normalize(hitPos - m_center);
  return {t, hitPos, normal};
}

struct Material
Sphere::
getMaterial() {
  return m_material;
}
