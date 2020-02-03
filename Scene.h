#ifndef SCENE_H_
#define SCENE_H_ 

#include <vector>
#include <memory>

#include "Ray.h"
#include "RenderableObject.h"

class Scene
{
  public:
    /// Ray hit with t not exceeding this amount is treated as
    /// an object hitting itself, and thus doesn't count as hitting
    /// another object
    static const float k_selfIntersectionBias;

    void addObject(std::unique_ptr<RenderableObject> _object);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Find the first object hit by the given ray
    /// @param[in]  _ray     The ray to cast
    /// @param[out] _hitInfo Address of a struct to store the hit info
    /// @return     The pointer to the first object hit by the ray, or nullptr
    ///             if no object is hit.
    RenderableObject* firstRayHit(Ray _ray, RayHit* _hitInfo);

  private:
    std::vector<std::unique_ptr<RenderableObject>> m_objects;
};

#endif // SCENE_H_