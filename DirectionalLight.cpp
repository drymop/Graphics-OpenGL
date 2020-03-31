#include "DirectionalLight.h"

#include <limits>

LightRay
DirectionalLight::
getLightRay(glm::vec3 _destination) const {
  return {
    m_dir, 
    std::numeric_limits<float>::infinity(), // infinitely far away
    m_intensityAmbient, 
    m_intensityDiffuse, 
    m_intensitySpecular,
  };
}

void 
DirectionalLight::
sendLightData(LightUniformLocations locations) const {
  glUniform1i(locations.type, 3); // type 3 is directional light
  glUniform3fv(locations.dir, 1, glm::value_ptr(m_dir));
  glUniform3fv(locations.intensityAmbient,  1, glm::value_ptr(m_intensityAmbient));
  glUniform3fv(locations.intensityDiffuse,  1, glm::value_ptr(m_intensityDiffuse));
  glUniform3fv(locations.intensitySpecular, 1, glm::value_ptr(m_intensitySpecular));
}