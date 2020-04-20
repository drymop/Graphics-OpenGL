#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "RasterizableObject.h"

struct Particle {
  glm::vec3 p;     ///< position
  glm::vec3 v;     ///< velocity
  glm::vec3 a;     ///< aceleration
  glm::vec3 color; ///< color
  float age;       ///< remaining life time
};

class ParticleSystem : public RasterizableObject
{
  public:
    /// Max number of particles in the system
    static const int MAX_N_PARTICLES =  100;

    ParticleSystem();

    void sendMeshData() override;

    void draw() override;

    void update(float deltaTime) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Ray-tracing is not supported for now, so intersectRay nevers returns a
    /// valid RayHit
    RayHit intersectRay(Ray _ray) const override {
      return RayHit();
    }

  private:
    /// pool of particles, partitioned into alive and dead particles
    Particle m_particles[MAX_N_PARTICLES];
    /// number of currently alive particles
    int m_nParticles;
    /// VBO used to store particles info
    GLuint m_vbo;
    /// Buffer to transfer particle positions
    glm::vec3 m_buffer[MAX_N_PARTICLES];
};

#endif // PARTICLE_SYSTEM_H_