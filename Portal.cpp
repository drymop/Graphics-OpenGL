#include "Portal.h"

Portal::
Portal(glm::vec3 _center1, float _radius1, glm::vec3 _normal1, glm::vec3 _up1,
      glm::vec3 _center2, float _radius2, glm::vec3 _normal2, glm::vec3 _up2) 
  : sides{{
      {{_center1, _radius1, _normal1, {}}, _up1, glm::vec3()}, 
      {{_center2, _radius2, _normal2, {}}, _up2, glm::vec3()}
    }}
{
  for(auto& s : sides) {
    // create a orthonormal basis of right, up, normal for each side
    s.right = glm::normalize(glm::cross(s.up, s.circle.getNormal()));
    s.up = glm::normalize(glm::cross(s.circle.getNormal(), s.right));
  }
}

RayHit 
Portal::
intersectRay(Ray _ray) const {
  RayHit hit = sides[0].circle.intersectRay(_ray);
  if (hit.t > 0) {
    return transformHit(sides[0], sides[1], hit, _ray.getDirection());
  }
  hit = sides[1].circle.intersectRay(_ray);
  if (hit.t > 0) {
    return transformHit(sides[1], sides[0], hit, _ray.getDirection());
  }
  return RayHit();
}

RayHit
Portal::
transformHit(const PortalSide& inSide, 
    const PortalSide& outSide, const RayHit& hit, glm::vec3 viewDir) const
{
  RayHit transformedHit;
  transformedHit.t = hit.t;
  // transform the position from the hit side to the other side
  glm::vec3 p = hit.position;
  transformedHit.position = 
      outSide.circle.getRadius() / inSide.circle.getRadius() *
      (glm::dot(p, inSide.up) * outSide.up
      + glm::dot(p, inSide.right) * outSide.right);

  glm::vec3 view2 = glm::normalize(
      glm::dot(viewDir, inSide.up) * outSide.up
      - glm::dot(viewDir, inSide.right) * outSide.right
      - glm::dot(viewDir, inSide.circle.getNormal()) * outSide.circle.getNormal()
  );
  transformedHit.normal = glm::normalize(view2 - viewDir);
  return transformedHit;
}