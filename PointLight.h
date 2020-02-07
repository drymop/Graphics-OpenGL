#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "LightSource.h"

////////////////////////////////////////////////////////////////////////////////
/// A point that emits light equally in all direction
class PointLight : public LightSource
{
  public:
    PointLight(glm::vec3 _pos, 
        glm::vec3 _intensity_ambient,
        glm::vec3 _intensity_diffuse, 
        glm::vec3 _intensity_specular) 
    : m_pos(_pos),
      m_intensity_ambient(_intensity_ambient),
      m_intensity_diffuse(_intensity_diffuse),
      m_intensity_specular(_intensity_specular)
      {};
    
    ////////////////////////////////////////////////////////////////////////////
    /// @return LightRay from this light source to the destination
    struct LightRay getLightRay(glm::vec3 _destination) const;

  private:
    glm::vec3 m_pos;
    glm::vec3 m_intensity_ambient;
    glm::vec3 m_intensity_diffuse;
    glm::vec3 m_intensity_specular;
};

#endif // POINT_LIGHT_H_