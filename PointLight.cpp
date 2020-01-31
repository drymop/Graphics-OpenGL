#include "PointLight.h"

// vector library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

PointLight::
PointLight(glm::vec3 _pos, 
    glm::vec3 _intensity_diffuse, 
    glm::vec3 _intensity_specular)
    : m_pos(_pos),
      m_intensity_diffuse(_intensity_diffuse),
      m_intensity_specular(_intensity_specular)
{}


struct LightRay
PointLight::
getLightRay(glm::vec3 _destination) {
  glm::vec3 dir = glm::normalize(_destination - m_pos);
  return {dir, m_intensity_diffuse, m_intensity_specular};
}
