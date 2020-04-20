#ifndef PARTICLE_GRAVITY_H_
#define PARTICLE_GRAVITY_H_

#include <glm/glm.hpp>
#include "ParticleForce.h"

////////////////////////////////////////////////////////////////////////////////
/// Represent a constant acceleration toward a direction
////////////////////////////////////////////////////////////////////////////////
class ParticleGravity : public ParticleForce
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct gravity with direction and magnitude specified by the
    /// vector _g
    ParticleGravity(glm::vec3 _g) : m_g(_g) {};

    void applyForce(Particle& particle) const override {
      particle.force += m_g * particle.m;
    }

  private:
    glm::vec3 m_g;
};

#endif // PARTICLE_GRAVITY_H_