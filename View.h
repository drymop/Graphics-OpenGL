#ifndef VIEW_H_
#define VIEW_H_

#include "Camera.h"
#include "Ray.h"

class View
{
  public:
    virtual int getFrameWidth() const = 0;
    virtual int getFrameHeight() const = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create a ray through the specified pixel
    ///
    /// The pixel coordinate is as follow: (0, 0) is at the bottom left of the 
    /// frame, with the x axis along the width of the frame, and the y axis along
    /// the height of the frame
    /// @param _cam     Camera to shoot ray from
    /// @param _pixelX  X-coordinate of the pixel
    /// @param _pixelY  Y-coordinate of the pixel
    /// @param _xJitter Deviation in X-coordinate from the center of the pixel
    /// @param _yJitter Deviation in Y-coordinate from the center of the pixel
    virtual Ray castRay(const Camera& _cam, int _pixelX, int _pixelY, float _xJitter, float _yJitter) const = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Generate the projection matrix
    virtual glm::mat4 getProjectionMatrix() const = 0;


    ////////////////////////////////////////////////////////////////////////////
    /// @brief Resize the view as the physical frame size changes
    virtual void resize(int _frame_width, int _frame_height) = 0;
};

#endif // VIEW_H_