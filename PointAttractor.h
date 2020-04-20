#ifndef POINT_ATTRACTOR_H_
#define POINT_ATTRACTOR_H_

#include <glm/glm.hpp>
#include "ParticleForce.h"

////////////////////////////////////////////////////////////////////////////////
/// Represent a force that attracts all particles toward a point
////////////////////////////////////////////////////////////////////////////////
class PointAttractor : public ParticleForce
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct an attractor toward a point, with coefficient g
    /// If g is negative, particles are repulsed from the point
    PointAttractor(glm::vec3 _point, float _g) : m_point(_point), m_g(_g) {};

    void applyForce(Particle& particle) const override;

  private:
    glm::vec3 m_point;
    float m_g;
};

#endif // POINT_ATTRACTOR_H_