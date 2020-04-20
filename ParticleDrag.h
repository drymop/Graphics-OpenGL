#ifndef PARTICLE_DRAG_H_
#define PARTICLE_DRAG_H_

#include <algorithm>
#include <glm/glm.hpp>
#include "ParticleForce.h"

////////////////////////////////////////////////////////////////////////////////
/// Represent a force that slows down particle proportional to squared velocity
////////////////////////////////////////////////////////////////////////////////
class ParticleDrag : public ParticleForce
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct a drag with coefficent k
    ParticleDrag(float _k) : m_k(_k) {};

    void applyForce(Particle& particle) const override {
      float speed = glm::length(particle.v);
      float t = std::min(m_k * speed, particle.m); // make sure drag doesn't cause negative speed
      particle.force -= t * particle.v;
    }

  private:
    float m_k;
};

#endif // PARTICLE_DRAG_H_