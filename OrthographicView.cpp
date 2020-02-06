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
castRay(int _pixelX, int _pixelY, float _xJitter, float _yJitter) {
  float x = m_planeLeft + m_planeWidth * (_pixelX + 0.5f + _xJitter) / m_frameWidth;
  float y = m_planeBottom + m_planeHeight * (_pixelY + 0.5f + _yJitter) / m_frameHeight;
  return Ray(glm::vec3(x, y, 0), glm::vec3(0, 0, -1));
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
}