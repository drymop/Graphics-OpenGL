#include "ParticleGenerator.h"

glm::vec3 
ParticleGenerator::
getRandomDirection() {
  std::uniform_real_distribution<> dis(-1.0, 1.0);
  glm::vec3 v{1.f, 1.f, 1.f};
  while (glm::length2(v) >= 1.f) {
    v = {dis(m_randGen), dis(m_randGen), dis(m_randGen)};
  }
  return glm::normalize(v);
}

float
ParticleGenerator::
jitterUniform(float lo, float hi) {
  std::uniform_real_distribution<> dis(lo, hi);
  return dis(m_randGen);
}