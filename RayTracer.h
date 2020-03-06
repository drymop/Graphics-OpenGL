#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include <future>
#include <vector>

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "Ray.h"
#include "Renderer.h"

class RayTracer : public Renderer
{
  public:
    RayTracer(int width, int height);

    RayTracer(const RayTracer& _other) = delete;

    void setFrameSize(int width, int height) override;

    void initScene(Scene& scene) override {};

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Render the scene under the given view (ortho/perspective) into the
    /// buffer.
    void render(const Scene& scene) override;

  private:
    std::unique_ptr<glm::vec4[]> m_frame{nullptr}; ///< Framebuffer
    
    const int MAX_RAY_RECURSION = 5;

    const std::vector<std::vector<glm::vec2>> ANTI_ALIAS_JITTERS {
      {
        {0.f, 0.f}
      },
      {
        { 0.25f,  0.25f},
        { 0.25f, -0.25f},
        {-0.25f,  0.25f},
        {-0.25f, -0.25f}
      }
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Ray trace to find the color of a pixel
    /// @param i Pixel index along the X axis
    /// @param j Pixel index along the Y axis
    /// @return RGBA color encoded in a vec4
    glm::vec4 renderPixel(const Scene& scene, int i, int j);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm and ideal specular reflection 
    glm::vec3 shade(const Scene& scene, Ray ray, int maxRecursion);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Shader function to compute color on an object using Blinn-Phong
    /// shading algorithm
    glm::vec3 shadeSurface(const Scene& scene, glm::vec3 pos, glm::vec3 normal, glm::vec3 viewDir, const Material& material);
};


#endif // RAY_TRACER_H_