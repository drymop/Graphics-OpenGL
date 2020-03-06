#include "OrthographicView.h"

#include <cmath>

OrthographicView::
OrthographicView(int _frameWidth, int _frameHeight, float _viewPlaneHeight)
  : m_frameWidth(_frameWidth),
    m_frameHeight(_frameHeight),
    m_planeHeight(_viewPlaneHeight)
{
  calculateViewingPlaneSize();
}

Ray
OrthographicView::
castRay(const Camera& _cam, int _pixelX, int _pixelY, float _xJitter, float _yJitter) const {
  float x = m_planeLeft + m_planeWidth * (_pixelX + 0.5f + _xJitter) / m_frameWidth;
  float y = m_planeBottom + m_planeHeight * (_pixelY + 0.5f + _yJitter) / m_frameHeight;
  glm::vec3 origin = x * _cam.getRight() + y * _cam.getUp() + _cam.getEye();
  return Ray(origin, _cam.getAt());
}

void 
OrthographicView::
resize(int _frameWidth, int _frameHeight) {
  m_frameWidth = _frameWidth;
  m_frameHeight = _frameHeight;
  calculateViewingPlaneSize();
}

void 
OrthographicView::
calculateViewingPlaneSize() {
  m_planeWidth = m_planeHeight * m_frameWidth / m_frameHeight;
  m_planeLeft = -m_planeWidth / 2;
  m_planeBottom = -m_planeHeight / 2;
  m_projMatrix = glm::ortho(m_planeLeft, -m_planeLeft, m_planeBottom, -m_planeBottom);
}