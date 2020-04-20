#ifndef LINE_ATTRACTOR_H_
#define LINE_ATTRACTOR_H_

#include <glm/glm.hpp>
#include "ParticleForce.h"

////////////////////////////////////////////////////////////////////////////////
/// Represent a force that attracts all particles toward a line
////////////////////////////////////////////////////////////////////////////////
class LineAttractor : public ParticleForce
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct an attractor toward a line, with coefficient g
    /// Line is defined as a point and a direction
    /// If g is negative, particles are repulsed from the line
    LineAttractor(glm::vec3 _point, glm::vec3 _dir, float _g) 
      : m_point(_point), m_dir(glm::normalize(_dir)), m_g(_g) {};

    void applyForce(Particle& particle) const override;

  private:
    glm::vec3 m_point;
    glm::vec3 m_dir;
    float m_g;
};

#endif // LINE_ATTRACTOR_H_