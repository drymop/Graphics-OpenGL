#include "PerspectiveView.h"

#include <cmath>

PerspectiveView::
PerspectiveView(int _frameWidth, int _frameHeight, float _fovY) 
  : m_frameWidth(_frameWidth),
    m_frameHeight(_frameHeight),
    m_fov(_fovY)
{
  calculateViewingPlaneSize();
}

Ray
PerspectiveView::
castRay(int _pixelX, int _pixelY) {
  float x = m_planeLeft + m_planeWidth * (_pixelX + 0.5f) / m_frameWidth;
  float y = m_planeBottom + m_planeHeight * (_pixelY + 0.5f) / m_frameHeight;
  return Ray(glm::vec3(0, 0, 0), glm::vec3(x, y, -1));
}

void 
PerspectiveView::
resize(int _frameWidth, int _frameHeight) {
  m_frameWidth = _frameWidth;
  m_frameHeight = _frameHeight;
  calculateViewingPlaneSize();
}

void 
PerspectiveView::
calculateViewingPlaneSize() {
  // assume that the viewing pane is 1 unit away from the camera
  // (focalLength = 1)
  float top = tan(m_fov / 2);
  m_planeBottom = -top;
  m_planeHeight = top * 2;
  // viewing plane follows the aspect ratio of the frame
  float right = top * m_frameWidth / m_frameHeight;
  m_planeLeft = -right;
  m_planeWidth = right * 2;
}
