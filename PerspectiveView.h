#ifndef PERSPECTIVE_VIEW_H_
#define PERSPECTIVE_VIEW_H_

#include "View.h"

class PerspectiveView : public View
{
  public:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Construct a perspective view with a given physical frame dimension
    /// and a FOV in the y-axis
    /// @param _frame_width  Width of the frame, in pixel
    /// @param _frame_height Height of the frame, in pixel
    /// @param _fovY          Field of view angle, in radian
    PerspectiveView(int _frame_width, int _frame_height, float _fovY);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Create a ray from the camera through the center of the virtual pixel
    /// on the viewing pane corresponding to the specified pixel
    ///
    /// The camera is assumed to be at (0, 0, 0).
    /// The pixel coordinate is as follow: (0, 0) is at the bottom left of the 
    /// frame, with the x axis along the width of the frame, and the y axis along
    /// the height of the frame.
    /// @param _pixelX X-coordinate of the pixel
    /// @param _pixelY Y-coordinate of the pixel
    Ray castRay(int _pixelX, int _pixelY);

  private:
    float m_plane_left;
    float m_plane_bottom;
    float m_plane_width;
    float m_plane_height;
    float m_frame_width;
    float m_frame_height;
};

#endif // PERSPECTIVE_VIEW_H_