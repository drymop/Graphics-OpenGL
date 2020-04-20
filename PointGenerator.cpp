#include "PointGenerator.h"

#include <algorithm>

int 
PointGenerator::
generate(Particle* particles, int nParticles, float deltaTime) {
  m_nextGenTime -= deltaTime;
  if (m_nextGenTime > 0.f) {
    // still in "cool down", temporarily stop generating particles
    return 0;
  }
  m_nextGenTime += m_period;
  int genSize = std::min(nParticles, m_genSize);
  for (int i = 0; i < genSize; i++) {
    randomParticle(particles[i]);
  }
  return genSize;
}

void
PointGenerator::
randomParticle(Particle& particle) {
  particle.p = m_point;
  particle.v = getRandomDirection() * m_speed * jitterUniform(0.9f, 1.1f);
  particle.m = m_mass * jitterUniform(0.9f, 1.1f);
  particle.age = m_age * jitterUniform(0.9f, 1.1f);
}