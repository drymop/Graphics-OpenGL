#include "Ray.h"

Ray::
Ray(glm::vec3 _origin, glm::vec3 _dir)
  : m_origin(_origin), m_dir(glm::normalize(_dir)) {};

glm::vec3
Ray::
getOrigin() {
  return m_origin;
}

glm::vec3
Ray::
getDirection() {
  return m_dir;
}