#include "PerspectiveView.h"

#include <cmath>

PerspectiveView::
PerspectiveView(int _frame_width, int _frame_height, float _fovY) {
  m_frame_width = _frame_width;
  m_frame_height = _frame_height;

  // assume that the viewing pane is 1 unit away from the camera
  float top = tan(_fovY / 2);
  m_plane_bottom = -top;
  m_plane_height = top * 2;
  // viewing plane follows the aspect ratio of the frame
  float right = top * (_frame_width / _frame_height);
  m_plane_left = -right;
  m_plane_width = right * 2;
}

Ray 
PerspectiveView::
castRay(int _pixelX, int _pixelY) {
  float x = m_plane_left + m_plane_width * (_pixelX + 0.5f) / m_frame_width;
  float y = m_plane_bottom + m_plane_height * (_pixelY + 0.5f) / m_frame_height;
  return Ray(glm::vec3(0, 0, 0), glm::vec3(x, y, -1));
}
