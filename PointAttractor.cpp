#include "PointAttractor.h"

void
PointAttractor::
applyForce(Particle& particle) const {
  glm::vec3 dir = m_point - particle.p;
  float dist = glm::length(dir);
  if (dist != 0) {
    dir /= dist;
    dist += 1.f; // make sure force doesn't approach infinity as dist -> 0
    float f = m_g * particle.m / (dist * dist);
    particle.force += f * dir;
  }
}