// STL
#include <cmath>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "Sphere.h"

Sphere::
Sphere(glm::vec3 _center, float _radius) 
  : m_center(_center), m_radius(_radius) {};


float 
Sphere::
intersect(Ray _ray) {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();
  glm::vec3 pMinusC = p - m_center;
  float bHalf = glm::dot(d, pMinusC);
  float c = glm::length2(pMinusC) - m_radius * m_radius;

  float discriminant = bHalf * bHalf - c;
  // no intersection
  if (discriminant <= 0) {
    return -1;
  }
  // return the smaller t
  return -bHalf - sqrt(discriminant);
}