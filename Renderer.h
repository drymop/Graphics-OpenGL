#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>

#include "OrthographicView.h"
#include "PerspectiveView.h"
#include "Scene.h"
#include "View.h"

class Renderer
{
  public:
    Renderer(int frameWidth, int frameHeight) 
      : m_width(frameWidth),
        m_height(frameHeight),
        m_hasAntiAlias(false),
        m_isPerspectiveView(true)
    {
      m_view = std::make_unique<PerspectiveView>(m_width, m_height, FOV);
    }

    virtual void setFrameSize(int width, int height) {
      m_width = width;
      m_height = height;
      m_view->resize(width, height);
    }

    virtual void setPerspectiveView(bool enabled) {
      if (m_isPerspectiveView == enabled) {
        return;
      }
      m_isPerspectiveView = enabled;
      if (m_isPerspectiveView) {
        m_view = std::make_unique<PerspectiveView>(m_width, m_height, FOV);
      } else {
        m_view = std::make_unique<OrthographicView>(m_width, m_height, ORTHO_VIEW_PLANE_HEIGHT);
      }
    }

    virtual void setAntiAlias(bool enabled) {
      m_hasAntiAlias = enabled;
    }

    bool isPerspectiveView() const { return m_isPerspectiveView; }
    bool hasAntiAlias() const { return m_hasAntiAlias; }

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Render the scene under the given view (ortho/perspective) into the
    /// buffer.
    virtual void render(const Scene& scene) = 0;

    virtual ~Renderer() {};

  protected:
    int m_width;
    int m_height;

    // View
    std::unique_ptr<View> m_view{nullptr};
    bool m_isPerspectiveView;
    const float FOV{glm::radians(60.f)};
    const float ORTHO_VIEW_PLANE_HEIGHT{5.f};

    bool m_hasAntiAlias;
};

#endif // RENDERER_H_