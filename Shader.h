#ifndef SHADER_H_
#define SHADER_H_

#include "Ray.h"
#include "Scene.h"

class Shader
{
  public:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm and ideal specular reflection 
    glm::vec3 shade(Scene& scene, Ray ray, int maxRecursion);

  private:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm
    glm::vec3 shadeSurface(const Scene& scene, glm::vec3 pos, glm::vec3 normal, glm::vec3 viewDir, const Material& material);
};


#endif // SHADER_H_