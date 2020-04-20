#ifndef PARTICLE_FORCE_H_
#define PARTICLE_FORCE_H_

#include "Particle.h"

class ParticleForce
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Apply a force to a particle
    virtual void applyForce(Particle& p) const = 0;  
};
#endif // PARTICLE_FORCE_H_