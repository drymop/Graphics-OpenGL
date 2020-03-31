#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "LightSource.h"

////////////////////////////////////////////////////////////////////////////////
/// A point that emits light equally in all direction
class PointLight : public LightSource
{
  public:
    PointLight(glm::vec3 _pos, 
        glm::vec3 _intensityAmbient,
        glm::vec3 _intensityDiffuse, 
        glm::vec3 _intensitySpecular) 
    : m_pos(_pos),
      m_intensityAmbient(_intensityAmbient),
      m_intensityDiffuse(_intensityDiffuse),
      m_intensitySpecular(_intensitySpecular)
      {};

    glm::vec3 getPosition() const { return m_pos; }
    glm::vec3 getIa() const { return m_intensityAmbient; }
    glm::vec3 getId() const { return m_intensityDiffuse; }
    glm::vec3 getIs() const { return m_intensitySpecular; }
    
    ////////////////////////////////////////////////////////////////////////////
    /// @return LightRay from this light source to the destination
    LightRay getLightRay(glm::vec3 _destination) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Send light uniform data to the rasterizer
    void sendLightData(LightUniformLocations _locs) const;


  private:
    glm::vec3 m_pos;
    glm::vec3 m_intensityAmbient;
    glm::vec3 m_intensityDiffuse;
    glm::vec3 m_intensitySpecular;
};

#endif // POINT_LIGHT_H_