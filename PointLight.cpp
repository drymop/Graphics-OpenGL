#include "PointLight.h"

LightRay
PointLight::
getLightRay(glm::vec3 _destination) const {
  glm::vec3 v = _destination - m_pos;
  float d = glm::length(v);
  float attenuation = m_linearAttenuation[0] + m_linearAttenuation[1]*d + m_linearAttenuation[2]*d*d;
  return {
    v/d, 
    d,
    m_intensityAmbient/attenuation, 
    m_intensityDiffuse/attenuation, 
    m_intensitySpecular/attenuation,
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