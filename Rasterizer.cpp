#include "Rasterizer.h"

#include "CompileShaders.h"
#include "GLInclude.h"
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
  for(auto& light : scene.lightSources()) {
    // light uniform name has format 
    //   lights[i].attribute
    std::string prefix = "lights[" + std::to_string(i++) + "].";
    LightUniformLocations uniformLocations {
      getUniformLocation(prefix + "type"),
      getUniformLocation(prefix + "pos"),
      getUniformLocation(prefix + "dir"),
      getUniformLocation(prefix + "cutoff"),
      getUniformLocation(prefix + "ia"),
      getUniformLocation(prefix + "id"),
      getUniformLocation(prefix + "is"),
      getUniformLocation(prefix + "al"),
      getUniformLocation(prefix + "aa"),
    };
    light->sendLightData(uniformLocations);
  }
  glUniform1i(getUniformLocation("numLights"), i);
  
  // Save uniform locations for each object
  ObjectUniformLocations objUniformLocs {
    getUniformLocation("vertexModelMatrix"),
    getUniformLocation("normalModelMatrix"),
    {
      getUniformLocation("material.ka"),
      getUniformLocation("material.kd"),
      getUniformLocation("material.ks"),
      getUniformLocation("material.shininess")
    }
  };
  for(auto& obj : scene.rasterizableObjects()) {
    obj->setUniformLocations(objUniformLocs);
  }
}

void
Rasterizer::
render(const Scene& scene) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Set up camera
  const Camera& camera = scene.getCamera();
  vec3 eye = camera.getEye();
  glUniform3f(getUniformLocation("cameraPos"), eye.x, eye.y, eye.z);
  mat4 viewMatrix = glm::lookAt(eye, eye + camera.getAt(), camera.getUp());
  mat4 projMatrix = m_view->getProjectionMatrix();
  glUniformMatrix4fv(
      getUniformLocation("viewProjectionMatrix"), 
      1, GL_FALSE, glm::value_ptr(projMatrix * viewMatrix));
  // Draw
  for(auto& obj : scene.rasterizableObjects()) {
    obj->draw();
  }
}

GLint
Rasterizer::
getUniformLocation(const std::string& uniformName) {
  return glGetUniformLocation(m_program, uniformName.c_str());
}
