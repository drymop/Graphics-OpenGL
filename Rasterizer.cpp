#include "Rasterizer.h"

using glm::vec3, glm::mat4;


void
Rasterizer::
render(const Scene& scene) {
  // Set up camera
  const Camera& camera = scene.getCamera();
  vec3 eye = camera.getEye();
  glUniform3f(glGetUniformLocation(m_program, "cameraPos"), eye.x, eye.y, eye.z);
  mat4 viewMatrix = glm::lookAt(eye, eye + at, up);
  glUniformMatrix4fv(
      glGetUniformLocation(m_program, "viewProjectionMatrix"), 
      1, GL_FALSE, glm::value_ptr(projMatrix * viewMatrix));

  // Draw
  for(auto& obj : scene.rasterizableObjects()) {
    obj->draw();
  }
}