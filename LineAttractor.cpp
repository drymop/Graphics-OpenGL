#include "LineAttractor.h"

using glm::vec3, glm::dot;

void
LineAttractor::
applyForce(Particle& particle) const {
  // find the closest point in line to the particle
  vec3 closest = m_point + dot(particle.p - m_point, m_dir) * m_dir;
  vec3 dir = closest - particle.p;
  float dist = glm::length(dir);
  if (dist != 0) {
    dir /= dist;
    dist += 1.f; // make sure force doesn't blow up to infinity as dist -> 0
    float f = m_g * particle.m / (dist * dist);
    particle.force += f * dir;
  }
}