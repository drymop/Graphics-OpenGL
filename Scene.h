#ifndef SCENE_H_
#define SCENE_H_ 

#include <vector>
#include <memory>

#include "LightSource.h"
#include "Ray.h"
#include "RenderableObject.h"

class Scene
{
  public:
    /// Ray hit with t not exceeding this amount is treated as
    /// an object hitting itself, and thus doesn't count as hitting
    /// another object
    static const float SELF_INTERSECTION_BIAS;

    void addObject(std::unique_ptr<RenderableObject> _object);

    void setAmbientLight(glm::vec3 _intensity);

    void addLightSource(std::unique_ptr<LightSource> _light);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Find the first object hit by the given ray
    /// @param[in]  _ray     The ray to cast
    /// @param[out] _hitInfo Address of a struct to store the hit info
    /// @return     The pointer to the first object hit by the ray, or nullptr
    ///             if no object is hit.
    RenderableObject* firstRayHit(Ray _ray, RayHit* _hitInfo) const;

    ////////////////////////////////////////////////////////////////////////////
    /// @return The intensity of ambient light in this scene
    glm::vec3 getAmbientLight() const;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Provide an iterable of light sources
    std::vector<LightSource*> lightSources() const;

  private:
    std::vector<std::unique_ptr<RenderableObject>> m_objects;
    glm::vec3 ambient_intensity;
    std::vector<std::unique_ptr<LightSource>> m_lights;
};

#endif // SCENE_H_