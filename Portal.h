#ifndef PORTAL_H_
#define PORTAL_H_

#include <array>

#include "Circle.h"

class Portal : public RenderableObject
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// Construct the Portal from 2 circles with defined up direction
    Portal(glm::vec3 _center1, float _radius1, glm::vec3 _normal1, glm::vec3 _up1,
      glm::vec3 _center2, float _radius2, glm::vec3 _normal2, glm::vec3 _up2);

    RayHit intersectRay(Ray _ray) const override;

    Material getMaterial() const override {
      // pure reflection (aka get color from the other side's view)
      return {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, {1, 1, 1}};
    }

  private:
    struct PortalSide {
      Circle circle;
      glm::vec3 up;
      glm::vec3 right;
    };

    std::array<PortalSide, 2> sides;

    ////////////////////////////////////////////////////////////////////////////
    /// Transform the hit info of the ray hit from inSide such that the
    /// reflected ray starts from the other side (outSide) and head in the
    /// correct direction
    RayHit transformHit(const PortalSide& inSide, 
        const PortalSide& outSide, const RayHit& hit, glm::vec3 viewDir) const;
};

#endif // PORTAL_H_