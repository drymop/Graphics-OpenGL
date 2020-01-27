#ifndef VIEW_H_
#define VIEW_H_

#include "Ray.h"

class View
{
  public:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Create a ray through the center of the specified pixel
    ///
    /// The pixel coordinate is as follow: (0, 0) is at the bottom left of the 
    /// frame, with the x axis along the width of the frame, and the y axis along
    /// the height of the frame
    /// @param _pixelX X-coordinate of the pixel
    /// @param _pixelY Y-coordinate of the pixel
    virtual Ray castRay(int _pixelX, int _pixelY) = 0;
};

#endif // VIEW_H_