#include "ParticleSystem.h"
#include "PointAttractor.h"
#include "LineAttractor.h"
#include "ParticleDrag.h"

using glm::vec3, glm::mat4;

////////////////////////////////////////////////////////////////////////////////
/// Helper function to generate material for particles
MaterialConfig 
generateMaterial(vec3 color) {
  MaterialConfig m{};
  m.hasKdMap = m.hasKsMap = m.hasKeMap = false;
  m.defaultMaterial.ke = color;
  return m;
}

ParticleSystem::
ParticleSystem(
    glm::vec3 _color,
    float _interraction,
    std::vector<std::unique_ptr<ParticleForce>>&& _particleForces
)
  : RasterizableObject(
      Mesh(), generateMaterial(_color), mat4(1.f)
    ),
    m_nParticles(100),
    m_interraction_coef(_interraction),
    m_particleForces(std::move(_particleForces))
{
  int pInd = 0;
  for(int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      m_particles[pInd].p = {i, j, -10.f};
      m_particles[pInd].m = 1.f;
      m_particles[pInd].age = i*10 + 20*j;
      m_particles[pInd].color = {1.f, 1.f, 1.f};
      m_particles[pInd].v = {0.f, 0.f, 0.f};
      pInd++;
    }
  }
}

void
ParticleSystem::
sendMeshData() {
  // Create vertex array object
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // Create buffer to store particles data
  GLuint vbo;
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(vec3) * MAX_N_PARTICLES, 
               nullptr, 
               GL_STREAM_DRAW);

  // Specify vertex attributes within buffer
  // Since we only care about particle position,
  // normal and texture coordinates are disabled

  // - positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // - normals
  glDisableVertexAttribArray(1);
  // - textures
  glDisableVertexAttribArray(2);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void
ParticleSystem::
draw() {
  // send object uniform data
  sendUniformData();
  // send particles' positions to shader
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  for (int i = 0; i < m_nParticles; i++) {
    m_buffer[i] = m_particles[i].p;
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_nParticles, m_buffer, GL_STREAM_DRAW);

  // set default value for normals and textures, since they are not supplied in buffer
  glVertexAttrib3f(1, 0.f, 0.f, 0.f);
  glVertexAttrib2f(2, 0.f, 0.f);

  // draw
  glDrawArrays(GL_POINTS, 0, m_nParticles);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void
ParticleSystem::
update(float deltaTime) {
  // iterate backward to remove dead particles
  for (int i = m_nParticles-1; i>=0; i--) {
    m_particles[i].age -= deltaTime;
    m_particles[i].force = {0.f, 0.f, 0.f};
    if (m_particles[i].age <= 0) {
      m_nParticles--;
      std::swap(m_particles[i], m_particles[m_nParticles]);
    }
  }

  // particle interactions
  for (int i = 0; i < m_nParticles; i++) {
    for (int j = 0; j < i; j++) {
      Particle& p1 = m_particles[i];
      Particle& p2 = m_particles[j];
      vec3 dir = p2.p - p1.p;
      float dist = glm::length(dir);
      dir /= dist;
      float f = m_interraction_coef * p1.m * p2.m / dist / dist;
      p1.force += f * dir;
      p2.force -= f * dir;
    }
  }

  // // gravity
  // vec3 gravity = {0.f, -0.01f, 0.f};
  // for (int i = 0; i < m_nParticles; i++) {
  //   m_particles[i].force += m_particles[i].m * gravity;
  // }

  // point attractor
  for(auto& force : m_particleForces) {
    for (int i = 0; i < m_nParticles; i++) {
      force->applyForce(m_particles[i]);
    }
  }

  // final update
  for (int i = 0; i < m_nParticles; i++) {
    m_particles[i].p += m_particles[i].v * deltaTime;
    m_particles[i].v += m_particles[i].force / m_particles[i].m;
  }
}
