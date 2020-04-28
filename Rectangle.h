#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "RasterizableObject.h"


class Rectangle : public RasterizableObject
{
  public:
    /// Create rectangle from the bottom left conner point,
    /// The up direction (can be longer than 1)
    /// And the right direction
    Rectangle(
      glm::vec3 _bottomLeft,
      glm::vec3 _right,
      glm::vec3 _up,
      const MaterialConfig& _material
    );

  private:
    static Mesh generateMesh();
    
    static glm::mat4 generateTransform(
      glm::vec3 _bottomLeft,
      glm::vec3 _right,
      glm::vec3 _up
    );
};

#endif // RECTANGLE_H_