#ifndef PARTICLE_GENERATOR_H_
#define PARTICLE_GENERATOR_H_

#include <random>
#include "GLInclude.h"
#include "Particle.h"

class ParticleGenerator
{
  public:
    ParticleGenerator() : m_randGen(std::random_device{}()) {};
    ////////////////////////////////////////////////////////////////////////////
    /// Generate new particles from the pool of dead particles, and put the
    /// new particles at the start of the array.
    /// @return number of particles generated.
    virtual int generate(Particle* particles, int nParticles, float deltaTime) = 0;

  protected:
    std::mt19937 m_randGen;

    glm::vec3 getRandomDirection();

    float jitterUniform(float lo, float hi);
};

#endif // PARTICLE_GENERATOR_H_