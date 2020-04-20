#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>

struct Particle {
  glm::vec3 p;       ///< position
  glm::vec3 v;       ///< velocity
  glm::vec3 force;   ///< force
  float     m;       ///< mass
  glm::vec3 color;   ///< color
  float age;         ///< remaining life time
};

#endif // PARTICLE_H_