#include <vector>
#include "RasterizableObject.h"

////////////////////////////////////////////////////////////////////////////////
/// A Bezier surface represented by 16 control points
class BezierSurface : public RasterizableObject
{
  public:
    BezierSurface(
      const std::vector<glm::vec3>& _controlPoints,
      const MaterialConfig& _materialConfig,
      const glm::mat4& _transform);

  private:
    static Mesh generateMesh(const std::vector<glm::vec3>& _controlPoints, int _prec);
};