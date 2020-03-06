#include "Rasterizer.h"

#include <string>

#include "CompileShaders.h"
#include "PointLight.h"

using glm::vec3, glm::mat4;

Rasterizer::
Rasterizer(int frameWidth, int frameHeight)
  : Renderer(frameWidth, frameHeight) 
{
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  m_program = compileProgram("shaders/phong.vert",
                             "shaders/phong.frag");
  glUseProgram(m_program);
}

void
Rasterizer::
initScene(Scene& scene) {
  // Send lights
  int i = 0;
  for(auto& lightSource : scene.lightSources()) {
    PointLight* light = dynamic_cast<PointLight*>(lightSource);
    if (light == nullptr) {
      // ignore non-point light
      continue;
    }
    std::string lightIndex = "lights[" + std::to_string(i++) + "].";
    std::string posName = lightIndex + "pos";
    glUniform3fv(
      glGetUniformLocation(m_program, posName.c_str()), 1,
      glm::value_ptr(light->getPosition()));
    std::string iaName = lightIndex + "ia";
    glUniform3fv(
      glGetUniformLocation(m_program, iaName.c_str()), 1,
      glm::value_ptr(light->getIa()));
    std::string idName = lightIndex + "id";
    glUniform3fv(
      glGetUniformLocation(m_program, idName.c_str()), 1,
      glm::value_ptr(light->getId()));
    std::string isName = lightIndex + "is";
    glUniform3fv(
      glGetUniformLocation(m_program, isName.c_str()), 1,
      glm::value_ptr(light->getIs()));
  }
  glUniform1i(glGetUniformLocation(m_program, "numLights"), i);
  // Save material uniform location for each object
  MaterialUniformLocation materialLoc {
    glGetUniformLocation(m_program, "material.ka"),
    glGetUniformLocation(m_program, "material.kd"),
    glGetUniformLocation(m_program, "material.ks"),
    glGetUniformLocation(m_program, "material.shininess")
  };
  GLint vModelToWorldLoc = glGetUniformLocation(m_program, "vertexModelMatrix");
  GLint nModelToWorldLoc = glGetUniformLocation(m_program, "normalModelMatrix");
  for(auto& obj : scene.rasterizableObjects()) {
    obj->setMaterialUniformLocation(materialLoc);
    obj->setVertexModelMatrixUniformLocation(vModelToWorldLoc);
    obj->setNormalModelMatrixUniformLocation(nModelToWorldLoc);
  }
}

void
Rasterizer::
render(const Scene& scene) {
  // Set up camera
  const Camera& camera = scene.getCamera();
  vec3 eye = camera.getEye();
  glUniform3f(glGetUniformLocation(m_program, "cameraPos"), eye.x, eye.y, eye.z);
  mat4 viewMatrix = glm::lookAt(eye, eye + camera.getAt(), camera.getUp());
  mat4 projMatrix = m_view->getProjectionMatrix();
  glUniformMatrix4fv(
      glGetUniformLocation(m_program, "viewProjectionMatrix"), 
      1, GL_FALSE, glm::value_ptr(projMatrix * viewMatrix));

  // Draw
  for(auto& obj : scene.rasterizableObjects()) {
    obj->draw();
  }
}