#ifndef SPHERE_H_
#define SPHERE_H_

#include "RenderableObject.h"

class Sphere : public RenderableObject
{
  public:
    Sphere(glm::vec3 _center, float _radius, Material _material);

    RayHit intersectRay(Ray _ray);

    struct Material getMaterial();

  private:
    /// Center of the sphere
    glm::vec3 m_center;
    /// Radius of the sphere. Must be a positive number.
    float m_radius;
    /// Material on the surface of this sphere
    struct Material m_material;
};

#endif // SPHERE_H_