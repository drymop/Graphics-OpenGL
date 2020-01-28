////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

// GL
#define GL_GLEXT_PROTOTYPES
#if   defined(OSX)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#elif defined(LINUX)
#include <GL/glut.h>
#endif

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "OrthographicView.h"
#include "PerspectiveView.h"
#include "Plane.h"
#include "Sphere.h"
#include "RenderableObject.h"
#include "View.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these

// Window
int g_width{1280};
int g_height{720};
int g_window{0};
std::unique_ptr<glm::vec4[]> g_frame{nullptr}; ///< Framebuffer

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

// Define view
std::unique_ptr<View> g_view{nullptr};
bool g_isPerspectiveView;
// Perspective view
float g_fov{1.0472}; // radians (about 60 degrees)
// Orthographic view
float g_orthoViewPlaneHeight{5.f};

// Objects to render
std::vector<std::unique_ptr<RenderableObject>> g_objects;

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void
initialize() {
  glClearColor(0.f, 0.f, 0.f, 0.f);

  g_frame = std::make_unique<glm::vec4[]>(g_width*g_height);
  g_isPerspectiveView = true;
  g_view = std::make_unique<PerspectiveView>(g_width, g_height, g_fov);

  // initialize objects
  g_objects.emplace_back(new Sphere(glm::vec3(2, 0, -10), 2));
  g_objects.emplace_back(new Sphere(glm::vec3(-2, 0, -5), 2));
  g_objects.emplace_back(new Plane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void
resize(GLint _w, GLint _h) {
  g_width = _w;
  g_height = _h;

  // Viewport
  glViewport(0, 0, g_width, g_height);
  // resize frame buffer
  g_frame = std::make_unique<glm::vec4[]>(g_width*g_height);
  // reset view to match new aspect ration
  g_view->resize(g_width, g_height);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Timer function to fix framerate in a GLUT application
/// @param _v Value (not used here)
///
/// Note, this is rudametary and fragile.
void
timer(int _v) {
  if(g_window != 0) {
    glutPostRedisplay();

    g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
    glutTimerFunc((unsigned int)(1000.f*g_delay), timer, 0);
  }
  else
    exit(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Ray trace to find the color of a pixel
/// @param i Pixel index along the X axis
/// @param j Pixel index along the Y axis
glm::vec4
renderPixel(int i, int j) {
  // cast ray
  Ray ray = g_view->castRay(i, j);
  float t = 99999999;
  // for now, as long as a sphere intersects this ray, return black
  // otherwise, return white
  for (auto& obj : g_objects) {
    if (obj->intersectRay(ray) > 0) {
      return glm::vec4(0, 0, 0, 0);
    }
  }
  return glm::vec4(1, 1, 1, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void
draw() {
  using namespace std::chrono;

  //////////////////////////////////////////////////////////////////////////////
  // Clear

  glm::vec4 bg_color(0.f, 0.4f, 0.f, 0.f);
  for(int i = 0; i < g_width*g_height; ++i)
    g_frame[i] = bg_color;

  //////////////////////////////////////////////////////////////////////////////
  // Draw

  for(int i = 0; i < g_width; i++) {
    for (int j = 0; j < g_height; j++) {
      g_frame[j * g_width + i] = renderPixel(i, j);
    }
  }

  glDrawPixels(g_width, g_height, GL_RGBA, GL_FLOAT, g_frame.get());

  //////////////////////////////////////////////////////////////////////////////
  // Show
  glutSwapBuffers();

  //////////////////////////////////////////////////////////////////////////////
  // Record frame time
  high_resolution_clock::time_point time = high_resolution_clock::now();
  g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
  g_frameTime = time;
  g_framesPerSecond = 1.f/(g_delay + g_frameRate);
  // printf("FPS: %6.2f\n", g_framesPerSecond);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
keyPressed(GLubyte _key, GLint _x, GLint _y) {
  switch(_key) {
    // Escape key : quit application
    case 27:
      std::cout << "Destroying window: " << g_window << std::endl;
      glutDestroyWindow(g_window);
      g_window = 0;
      break;
    // Space key: switch projection mode
    case ' ':
      g_isPerspectiveView = !g_isPerspectiveView;
      if (g_isPerspectiveView) {
        g_view = std::make_unique<PerspectiveView>(g_width, g_height, g_fov);
      } else {
        g_view = std::make_unique<OrthographicView>(g_width, g_height, g_orthoViewPlaneHeight);
      }
      break;
    // Unhandled
    default:
      std::cout << "Unhandled key: " << (int)(_key) << std::endl;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses of special keys
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
specialKeyPressed(GLint _key, GLint _x, GLint _y) {
  switch(_key) {
    // Arrow keys
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
      break;
    // Unhandled
    default:
      std::cout << "Unhandled special key: " << _key << std::endl;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int
main(int _argc, char** _argv) {
  //////////////////////////////////////////////////////////////////////////////
  // Initialize GLUT Window
  std::cout << "Initializing GLUTWindow" << std::endl;
  // GLUT
  glutInit(&_argc, _argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(g_width, g_height); // HD size
  g_window = glutCreateWindow("Spiderling: A Rudamentary Game Engine");

  // GL
  initialize();

  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glutReshapeFunc(resize);
  glutDisplayFunc(draw);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutTimerFunc(1000/FPS, timer, 0);

  // Start application
  std::cout << "Starting Application" << std::endl;
  glutMainLoop();

  return 0;
}

#if   defined(OSX)
#pragma clang diagnostic pop
#endif
