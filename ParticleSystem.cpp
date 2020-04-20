#include "ParticleSystem.h"

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
ParticleSystem() 
  : RasterizableObject(
      Mesh(), 
      generateMaterial(vec3(1.f, 0.f, 1.f)),
      mat4(1.f)
    ),
    m_nParticles(16)
{
  int pInd = 0;
  for(int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m_particles[pInd].p = {i, j, -10.f};
      m_particles[pInd].age = i + 2*j;
      m_particles[pInd].color = {1.f, 1.f, 1.f};
      m_particles[pInd].v = {1.f, 0.f, 0.f};
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
    m_particles[i].p += m_particles[i].v * deltaTime;
    m_particles[i].age -= deltaTime;
    if (m_particles[i].age <= 0) {
      m_nParticles--;
      std::swap(m_particles[i], m_particles[m_nParticles]);
    }
  }
}
