#include "PerspectiveView.h"

#include <cmath>

PerspectiveView::
PerspectiveView(int _frameWidth, int _frameHeight, float _fovY, float _near, float _far) 
  : m_frameWidth(_frameWidth),
    m_frameHeight(_frameHeight),
    m_fov(_fovY),
    m_planeNear(_near),
    m_planeFar(_far)
{
  calculateViewingPlaneSize();
}

Ray
PerspectiveView::
castRay(const Camera& _cam, int _pixelX, int _pixelY, float _xJitter, float _yJitter) const {
  float x = m_planeLeft + m_planeWidth * (_pixelX + 0.5f + _xJitter) / m_frameWidth;
  float y = m_planeBottom + m_planeHeight * (_pixelY + 0.5f + _yJitter) / m_frameHeight;
  glm::vec3 direction = x * _cam.getRight() + y * _cam.getUp() + _cam.getAt();
  return Ray(_cam.getEye(), direction);
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
  m_projMatrix = glm::perspective(
      m_fov, m_frameWidth/(float)m_frameHeight, m_planeNear, m_planeFar);
}
