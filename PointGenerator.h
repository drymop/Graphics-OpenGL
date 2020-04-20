#ifndef POINT_GENERATOR_H_
#define POINT_GENERATOR_H_

#include "ParticleGenerator.h"

class PointGenerator : public ParticleGenerator
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// Construct a generator from a point, with initial speed and random direction
    /// @param point   The point from which particles spawn
    /// @param speed   Initial speed of particles
    /// @param genSize Number of particles per generation
    /// @param period  Time between 2 generations
    PointGenerator(glm::vec3 point, float speed, float mass, float age, int genSize, float period) 
      : ParticleGenerator(),
        m_point(point), m_speed(speed), m_mass(mass), m_age(age), 
        m_genSize(genSize), m_period(period), m_nextGenTime(0.f)
    {}

    int generate(Particle* particles, int nParticles, float deltaTime);

  private:
    glm::vec3 m_point;
    float     m_speed;
    float     m_mass;
    float     m_age;
    int       m_genSize;
    float     m_period;
    float     m_nextGenTime; ///< Time until next generation

    void randomParticle(Particle& p);
};

#endif