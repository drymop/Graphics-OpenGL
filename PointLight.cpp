#include "PointLight.h"

// vector library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

struct LightRay
PointLight::
getLightRay(glm::vec3 _destination) const {
  glm::vec3 v = _destination - m_pos;
  return {
    glm::normalize(v), 
    glm::length(v), 
    m_intensity_ambient, 
    m_intensity_diffuse, 
    m_intensity_specular
  };
}
