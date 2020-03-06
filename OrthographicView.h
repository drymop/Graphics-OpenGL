#ifndef ORTHOGRAPHIC_VIEW_H_
#define ORTHOGRAPHIC_VIEW_H_

#include "View.h"

class OrthographicView : public View
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Construct a perspective view with a given physical frame dimension
    /// @param _frameWidth      Width of the frame, in pixel
    /// @param _frameHeight     Height of the frame, in pixel
    /// @param _viewPlaneHeight Height of the viewing plane, in virtual unit
    OrthographicView(int _frameWidth, int _frameHeight, float _viewPlaneHeight);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create a ray from the center of the virtual pixel (with jitter 
    /// applied) on the viewing pane, with direction parallel to the camera 
    /// facing direction (aka the -z axis)
    ///
    /// The pixel coordinate is as follow: (0, 0) is at the bottom left of the 
    /// frame, with the x axis along the width of the frame, and the y axis 
    /// along the height of the frame.
    /// @param _cam     Camera to shoot ray from
    /// @param _pixelX  X-coordinate of the pixel
    /// @param _pixelY  Y-coordinate of the pixel
    /// @param _xJitter Deviation in X-coordinate from the center of the pixel
    /// @param _yJitter Deviation in Y-coordinate from the center of the pixel
    Ray castRay(const Camera& _cam, int _pixelX, int _pixelY, float _xJitter, float _yJitter) const;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Generate the projection matrix
    glm::mat4 getProjectionMatrix() const { return m_projMatrix; }

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Resize the view as the physical frame size changes, keeping the
    /// same viewing plane height, but change the aspect ratio accordingly
    void resize(int _frameWidth, int _frameHeight);

  private:
    float m_planeLeft;
    float m_planeBottom;
    float m_planeWidth;
    float m_planeHeight;
    float m_frameWidth;
    float m_frameHeight;
    glm::mat4 m_projMatrix;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Calculate the size of the viewing plane based on the frame size
    /// and the viewing plane height
    void calculateViewingPlaneSize();
};

#endif // ORTHOGRAPHIC_VIEW_H_