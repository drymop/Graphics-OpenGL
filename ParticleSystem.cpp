#include "ParticleSystem.h"

ParticleSystem::
ParticleSystem() 
  : m_liveParticles(16)
{
  int pInd = 0;
  for(int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m_particles[pInd].p = {i, j, -10.f};
      m_particles[pInd].age = 100;
      m_particles[pInd].color = {1.f, 1.f, 1.f};
      pInd++;
    }
  }
}

void
ParticleSystem::
draw() const {

}