#ifndef LIGHT_SOURCE_H_
#define LIGHT_SOURCE_H_

#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////
/// Uniform locations to send light data to rasterizer
struct LightUniformLocations {
  GLint type;
  GLint pos;
  GLint dir;
  GLint cutoffDot;
  GLint intensityAmbient;
  GLint intensityDiffuse;
  GLint intensitySpecular;
  GLint linearAttenuation;
  GLint angleAttenuation;
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
    virtual LightRay getLightRay(glm::vec3 _destination) const = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// Send light uniform data to the rasterizer
    virtual void sendLightData(LightUniformLocations _locs) const = 0;
};

#endif // LIGHT_SOURCE_H_