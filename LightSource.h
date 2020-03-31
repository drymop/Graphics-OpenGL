#ifndef LIGHT_SOURCE_H_
#define LIGHT_SOURCE_H_

// vector library
#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
/// Uniform locations to send data to rasterizer
struct LightRay {
  glm::vec3 direction;
  float distance;
  glm::vec3 intensityAmbient;
  glm::vec3 intensityDiffuse;
  glm::vec3 intensitySpecular;
};


////////////////////////////////////////////////////////////////////////////////
/// Represent a light ray from a source to a point
struct LightRay {
  glm::vec3 direction;
  float distance;
  glm::vec3 intensityAmbient;
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
    virtual struct LightRay getLightRay(glm::vec3 _destination) const = 0;
};

#endif // LIGHT_SOURCE_H_