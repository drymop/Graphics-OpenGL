#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

#include <glm/glm.hpp>

#include "Material.h"
#include "Ray.h"

struct RayHit {
  /// How far along the ray is the hit
  float t;
  glm::vec3 position;
  glm::vec3 normal;
};

class RenderableObject
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @return Return the distance from the first point of this object hit by
    /// the ray to the origin of the ray. A non-positive return value indicates
    /// that the object is not hit by the ray.
    virtual RayHit intersectRay(Ray _ray) const = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// @return The material of this object.
    virtual struct Material getMaterial() const = 0;
};

#endif // RENDERABLE_OBJECT_H_