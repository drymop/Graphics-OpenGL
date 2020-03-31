#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_

#include "LightSource.h"

////////////////////////////////////////////////////////////////////////////////
/// A point that emits light equally in all direction
class DirectionalLight : public LightSource
{
  public:
    DirectionalLight(glm::vec3 _dir, 
        glm::vec3 _intensityAmbient,
        glm::vec3 _intensityDiffuse, 
        glm::vec3 _intensitySpecular) 
    : m_dir(glm::normalize(_dir)),
      m_intensityAmbient(_intensityAmbient),
      m_intensityDiffuse(_intensityDiffuse),
      m_intensitySpecular(_intensitySpecular)
      {};

    ////////////////////////////////////////////////////////////////////////////
    /// @return LightRay from this light source to the destination
    LightRay getLightRay(glm::vec3 _destination) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Send light uniform data to the rasterizer
    void sendLightData(LightUniformLocations _locs) const;

  private:
    glm::vec3 m_dir;
    glm::vec3 m_intensityAmbient;
    glm::vec3 m_intensityDiffuse;
    glm::vec3 m_intensitySpecular;
};

#endif // DIRECTIONAL_LIGHT_H_