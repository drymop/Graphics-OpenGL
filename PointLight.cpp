#include "PointLight.h"

LightRay
PointLight::
getLightRay(glm::vec3 _destination) const {
  glm::vec3 v = _destination - m_pos;
  return {
    glm::normalize(v), 
    glm::length(v), 
    m_intensityAmbient, 
    m_intensityDiffuse, 
    m_intensitySpecular
  };
}

void 
PointLight::
sendLightData(LightUniformLocations locations) const {
  glUniform1i(locations.type, 1); // type 1 is point light
  glUniform3fv(locations.pos, 1, glm::value_ptr(m_pos));
  glUniform3fv(locations.intensityAmbient,  1, glm::value_ptr(m_intensityAmbient));
  glUniform3fv(locations.intensityDiffuse,  1, glm::value_ptr(m_intensityDiffuse));
  glUniform3fv(locations.intensitySpecular, 1, glm::value_ptr(m_intensitySpecular));
}