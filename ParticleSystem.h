#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include <memory>

#include "Particle.h"
#include "ParticleForce.h"
#include "RasterizableObject.h"


class ParticleSystem : public RasterizableObject
{
  public:
    /// Max number of particles in the system
    static const int MAX_N_PARTICLES =  100;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create a particle system
    /// @param _color           Color of particles
    /// @param _interraction    Interraction coefficients between particles
    /// @param _particleForces  Global forces acting on all particles
    ParticleSystem(
        glm::vec3 _color,
        float _interraction,
        std::vector<std::unique_ptr<ParticleForce>>&& _particleForces);

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
    /// interraction coefficient between particles
    float m_interraction_coef;
    /// global forces acting on all particles
    std::vector<std::unique_ptr<ParticleForce>> m_particleForces;
    /// VBO used to store particles info
    GLuint m_vbo;
    /// Buffer to transfer particle positions
    glm::vec3 m_buffer[MAX_N_PARTICLES];
};

#endif // PARTICLE_SYSTEM_H_