#include "SpotLight.h"

#include <limits>

LightRay
SpotLight::
getLightRay(glm::vec3 _destination) const {
  // direction from light to destination, and distance
  glm::vec3 v = _destination - m_pos;
  float d = glm::length(v);
  v /= d;
  // check if within cone
  float dot = glm::dot(v, m_dir);
  if (dot <= m_cutoffDot) {
    // outside of light cone, return 0 for everything
    return LightRay();
  }
  // calculate attenuation = linear * angular
  float invLinear = m_linearAttenuation[0] + m_linearAttenuation[1]*d + m_linearAttenuation[2]*d*d;
  float attenuation = pow(dot, m_angleAttenuation) / invLinear;
  return {
    v, // direction
    d, // distance
    m_intensityAmbient * attenuation, 
    m_intensityDiffuse * attenuation,
    m_intensitySpecular * attenuation,
  };
}

void 
SpotLight::
sendLightData(LightUniformLocations locations) const {
  glUniform1i (locations.type, 2); // type 2 is spot light
  glUniform3fv(locations.pos, 1, glm::value_ptr(m_pos));
  glUniform3fv(locations.dir, 1, glm::value_ptr(m_dir));
  glUniform1f (locations.cutoffDot, m_cutoffDot);
  glUniform3fv(locations.intensityAmbient,  1, glm::value_ptr(m_intensityAmbient));
  glUniform3fv(locations.intensityDiffuse,  1, glm::value_ptr(m_intensityDiffuse));
  glUniform3fv(locations.intensitySpecular, 1, glm::value_ptr(m_intensitySpecular));
  glUniform3fv(locations.linearAttenuation, 1, glm::value_ptr(m_linearAttenuation));
  glUniform1f (locations.angleAttenuation, m_angleAttenuation);
}