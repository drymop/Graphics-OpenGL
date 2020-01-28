#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

#include "Ray.h"

class RenderableObject
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @return Return the distance from the first point of this object hit by
    /// the ray to the origin of the ray. A non-positive return value indicates
    /// that the object is not hit by the ray.
    virtual float intersectRay(Ray _ray) = 0;
};

#endif // RENDERABLE_OBJECT_H_