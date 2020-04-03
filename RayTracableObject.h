#ifndef RAYTRACABLE_OBJECT_H_
#define RAYTRACABLE_OBJECT_H_

#include "Ray.h"
#include "RenderableObject.h"

struct RayHit {
  /// How far along the ray is the hit
  float t;
  glm::vec3 position;
  glm::vec3 normal;
  Material  material;
};

class RayTracableObject : public RenderableObject
{
  public:
    RayTracableObject(const MaterialConfig& _m) : RenderableObject(_m) {};

    RayTracableObject(const Material& _m) : RenderableObject(_m) {};

    ////////////////////////////////////////////////////////////////////////////
    /// @return Return the distance from the first point of this object hit by
    /// the ray to the origin of the ray. A non-positive return value indicates
    /// that the object is not hit by the ray.
    virtual RayHit intersectRay(Ray _ray) const = 0;
};

#endif // RAYTRACABLE_OBJECT_H_