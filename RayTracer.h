#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "Ray.h"
#include "Scene.h"

class RayTracer
{
  public:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm and ideal specular reflection 
    glm::vec3 shade(const Scene& scene, Ray ray, int maxRecursion);

  private:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm
    glm::vec3 shadeSurface(const Scene& scene, glm::vec3 pos, glm::vec3 normal, glm::vec3 viewDir, const Material& material);
};


#endif // RAY_TRACER_H_