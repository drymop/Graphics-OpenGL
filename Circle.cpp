#include "Circle.h"

// GLM length2 function
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

RayHit
Circle::
intersectRay(Ray _ray) const {
  RayHit hit = Plane::intersectRay(_ray);
  if(glm::length2(hit.position - m_center) < m_radiusSq) {
    return hit; // collides within the circle
  }
  return RayHit(); // no collision
}