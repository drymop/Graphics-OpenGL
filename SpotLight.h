#ifndef SPOT_LIGHT_H_
#define SPOT_LIGHT_H_

#include "LightSource.h"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////
/// A point that emits light equally in all direction
class SpotLight : public LightSource
{
  public:
    SpotLight(
        glm::vec3 _pos,
        glm::vec3 _dir,
        float     _cutoffAngle, 
        glm::vec3 _intensityAmbient,
        glm::vec3 _intensityDiffuse, 
        glm::vec3 _intensitySpecular,
        glm::vec3 _linearAttenuation,
        float     _angleAttenuation) 
    : m_pos(_pos),
      m_dir(glm::normalize(_dir)),
      m_cutoffDot(cos(_cutoffAngle)),
      m_intensityAmbient(_intensityAmbient),
      m_intensityDiffuse(_intensityDiffuse),
      m_intensitySpecular(_intensitySpecular),
      m_linearAttenuation(_linearAttenuation),
      m_angleAttenuation(_angleAttenuation)
      {};

    ////////////////////////////////////////////////////////////////////////////
    /// @return LightRay from this light source to the destination
    LightRay getLightRay(glm::vec3 _destination) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Send light uniform data to the rasterizer
    void sendLightData(LightUniformLocations _locs) const;

  private:
    glm::vec3 m_pos;
    glm::vec3 m_dir;
    float     m_cutoffDot;
    glm::vec3 m_intensityAmbient;
    glm::vec3 m_intensityDiffuse;
    glm::vec3 m_intensitySpecular;
    glm::vec3 m_linearAttenuation;
    float     m_angleAttenuation;
};

#endif // SPOT_LIGHT_H_