#ifndef PERSPECTIVE_VIEW_H_
#define PERSPECTIVE_VIEW_H_

#include "View.h"

class PerspectiveView : public View
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct a perspective view with a given physical frame dimension
    /// and a FOV in the y-axis
    /// @param _frameWidth  Width of the frame, in pixel
    /// @param _frameHeight Height of the frame, in pixel
    /// @param _fovY        Field of view angle, in radian
    PerspectiveView(int _frameWidth, int _frameHeight, float _fovY);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create a ray from the camera through the center of the virtual 
    /// pixel (with some jitter) on the viewing pane corresponding to the 
    /// specified pixel.
    ///
    /// The camera is assumed to be at (0, 0, 0).
    /// The pixel coordinate is as follow: (0, 0) is at the bottom left of the 
    /// frame, with the x axis along the width of the frame, and the y axis 
    /// along the height of the frame.
    /// @param _pixelX X-coordinate of the pixel
    /// @param _pixelY Y-coordinate of the pixel
    /// @param _xJitter Deviation in X-coordinate from the center of the pixel
    /// @param _yJitter Deviation in Y-coordinate from the center of the pixel
    Ray castRay(int _pixelX, int _pixelY, float _xJitter, float _yJitter);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Resize the view as the physical frame size changes, keeping the
    /// same FOV
    void resize(int _frameWidth, int _frameHeight);

  private:
    float m_fov;
    float m_planeLeft;
    float m_planeBottom;
    float m_planeWidth;
    float m_planeHeight;
    float m_frameWidth;
    float m_frameHeight;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Calculate the size of the viewing plane based on the frame size
    /// and FOV
    void calculateViewingPlaneSize();
};

#endif // PERSPECTIVE_VIEW_H_