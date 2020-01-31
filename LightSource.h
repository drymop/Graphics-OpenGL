#ifndef LIGHT_SOURCE_H_
#define LIGHT_SOURCE_H_

// vector library
#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
/// Represent a light ray from a source
struct LightRay {
  glm::vec3 direction;
  glm::vec3 intensityDiffuse;
  glm::vec3 intensitySpecular;
};

////////////////////////////////////////////////////////////////////////////////
/// Abstract class defining a light source, with only a single method to
/// determine light intensity and direction at any point
class LightSource
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @return LightRay from this light source to the destination
    virtual struct LightRay getLightRay(glm::vec3 _destination) = 0;
};

#endif // LIGHT_SOURCE_H_