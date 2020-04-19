#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include <vector>
#include "GLInclude.h"

struct Particle {
  glm::vec3 p;     ///< position
  glm::vec3 v;     ///< velocity
  glm::vec3 a;     ///< aceleration
  glm::vec3 color; ///< color
  float age;       ///< remaining life time
};

class ParticleSystem
{
  public:
    /// Max number of particles in the system
    static const int MAX_N_PARTICLES =  100;

    ParticleSystem();

    void draw() const;

  private:
    /// pool of particles, partitioned into alive and dead particles
    Particle m_particles[MAX_N_PARTICLES];
    /// number of currently alive particles
    int m_liveParticles;
};

#endif // PARTICLE_SYSTEM_H_