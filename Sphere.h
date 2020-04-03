#ifndef SPHERE_H_
#define SPHERE_H_

#include "RasterizableObject.h"

class Sphere : public RayTracableObject
{
  public:
    Sphere(glm::vec3 _center, float _radius, const MaterialConfig& _matConfig) 
      : RayTracableObject(_matConfig),
        m_center(_center), 
        m_radius(_radius)
    {};

    RayHit intersectRay(Ray _ray) const override;

  private:
    /// Center of the sphere
    glm::vec3 m_center;
    /// Radius of the sphere. Must be a positive number.
    float m_radius;

    static Mesh generateMesh(int _prec);
};

#endif // SPHERE_H_