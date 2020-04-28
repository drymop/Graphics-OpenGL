#include "Rasterizer.h"

#include "CompileShaders.h"
#include "GLInclude.h"

#include <algorithm>
#include <vector>
#include "PointLight.h"

using glm::vec3, glm::mat4;

Rasterizer::
Rasterizer(int frameWidth, int frameHeight)
  : Renderer(frameWidth, frameHeight) 
{
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); // blending for transparency
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_program = compileProgram("shaders/phong.vert", 
                             "shaders/phong.frag");
  glUseProgram(m_program);
  glPointSize(3.0f);
}

void
Rasterizer::
initScene(Scene& scene) {
  // Set up texture samplers
  glUniform1i(getUniformLocation("kdTextureSampler"), 0);
  glUniform1i(getUniformLocation("ksTextureSampler"), 1);
  glUniform1i(getUniformLocation("keTextureSampler"), 2);
  glUniform1i(getUniformLocation("normalTextureSampler"), 3);
  glUniform1i(getUniformLocation("parallaxTextureSampler"), 4);

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
      getUniformLocation(prefix + "cutoffDot"),
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
    getUniformLocation("hasTransparency"),
    getUniformLocation("hasKdMap"),
    getUniformLocation("hasKsMap"),
    getUniformLocation("hasKeMap"),
    getUniformLocation("hasNormalMap"),
    getUniformLocation("hasParallaxMap"),
    {
      getUniformLocation("material.ka"),
      getUniformLocation("material.kd"),
      getUniformLocation("material.ks"),
      getUniformLocation("material.ke"),
      getUniformLocation("material.shininess"),
      getUniformLocation("material.transparency")
    }
  };
  for(auto& obj : scene.rasterizableObjects()) {
    obj->setUniformLocations(objUniformLocs);
    obj->sendMeshData();
  }
}

// Store an object and distance to camera, used for sorting
struct TransparentObject {
  float dist;
  RasterizableObject* obj;

  TransparentObject(float _dist, RasterizableObject* _obj)
    : dist(_dist), obj(_obj) {}

  bool operator<(const TransparentObject& other) const { 
    return dist > other.dist; 
  }
};

void
Rasterizer::
render(const Scene& scene) {
  glDepthMask(GL_TRUE);
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
  std::vector<TransparentObject> transparentObjs{};
  // First render all opaque objects
  for(auto& obj : scene.rasterizableObjects()) {
    if (obj->hasTransparency()) {
      // save transparent object for rendering later
      vec3 pos = obj->getRoughPosition();
      transparentObjs.emplace_back(glm::length2(eye-pos), obj);
    } else {
      // draw opaque object
      obj->draw();
    }
  }
  // Then render all transparent objects in sorted distance
  glDepthMask(GL_FALSE);
  std::sort(transparentObjs.begin(), transparentObjs.end());
  for(auto& tObj : transparentObjs) {
    tObj.obj->draw();
  }
}

GLint
Rasterizer::
getUniformLocation(const std::string& uniformName) {
  return glGetUniformLocation(m_program, uniformName.c_str());
}
