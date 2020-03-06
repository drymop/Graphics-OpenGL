////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <cfloat>
#include <chrono>
#include <cstdlib>
#include <future>
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

#include "LightSource.h"
#include "Material.h"
#include "OrthographicView.h"
#include "PerspectiveView.h"
#include "Plane.h"
#include "PointLight.h"
#include "Scene.h"
#include "SceneBuilder.h"
#include "Shader.h"
#include "Sphere.h"
#include "RayTracableObject.h"
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

// Parallelization
bool g_parallelize{true};
const unsigned int N_ROWS_PER_TASK = 16;

// Define view
std::unique_ptr<View> g_view{nullptr};
bool g_isPerspectiveView;
// Perspective view
const float FOV{1.0472}; // radians (about 60 degrees)
// Orthographic view
const float ORTHO_VIEW_PLANE_HEIGHT{5.f};

// Anti-aliasing
int g_antiAliasMode{0};
const std::vector<std::vector<glm::vec2>> g_antiAliasJitters {
  {
    {0.f, 0.f}
  },
  {
    { 0.25f,  0.25f},
    { 0.25f, -0.25f},
    {-0.25f,  0.25f},
    {-0.25f, -0.25f}
  }
};

// Scene to render
Scene g_scene;

// Shader
Shader g_shader;
const int N_SHADER_RECURSIONS = 5;
int g_maxShaderRecursion{0};

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize settings
void
initialize(const std::string& sceneFile) {
  glClearColor(0.f, 0.f, 0.f, 0.f);

  // initialize view
  g_frame = std::make_unique<glm::vec4[]>(g_width*g_height);
  g_isPerspectiveView = true;
  g_view = std::make_unique<PerspectiveView>(g_width, g_height, FOV);

  // initialize scene
  SceneBuilder sceneBuilder;
  g_scene = sceneBuilder.buildSceneFromJsonFile(sceneFile);
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
/// @return RGBA color encoded in a vec4
glm::vec4
renderPixel(int i, int j) {
  glm::vec3 color(0.f, 0.f, 0.f);
  for (auto& jitter : g_antiAliasJitters[g_antiAliasMode]) {
    // cast ray
    Ray ray = g_view->castRay(i, j, jitter.x, jitter.y);
    color += g_shader.shade(g_scene, ray, g_maxShaderRecursion);
  }
  color /= g_antiAliasJitters[g_antiAliasMode].size();
  return glm::vec4(color, 1);
}

void
renderRow(int j) {
  int pixel = j * g_width;
  for (int i = 0; i < g_width; i++) {
    g_frame[pixel++] = renderPixel(i, j);
  } 
}

void
renderTask(int startRow) {
  int endRow = std::min<int>(startRow + N_ROWS_PER_TASK, g_height);
  for (int j = startRow; j < endRow; j++) {
    renderRow(j);
  }
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

  if(g_parallelize) {
    // parallel version
    std::vector<std::future<void>> futures;
    futures.reserve(g_height);
    for (int j = 0; j < g_height; j += N_ROWS_PER_TASK) {
      auto fut = std::async(renderTask, j);
      futures.push_back(std::move(fut));
    }
    for(auto& fut : futures) {
      fut.wait();
    }
  } else {
    // serial version
    for (int j = 0; j < g_height; j++) {
      renderRow(j);
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
  printf("FPS: %6.2f\n", g_framesPerSecond);
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
    // A key: switch anti-aliasing mode
    case 'a':
      g_antiAliasMode = (g_antiAliasMode + 1) % g_antiAliasJitters.size();
      std::cout << "Anti-alias mode: " << g_antiAliasMode << std::endl;
      break;
    // P key: toggle parallelization
    case 'p':
      g_parallelize = !g_parallelize;
      std::cout << "Parallelization: " << g_parallelize << std::endl;
      break;
    // R key: toggle recursive ray-tracing
    case 'r':
      g_maxShaderRecursion = N_SHADER_RECURSIONS - g_maxShaderRecursion;
      std::cout << "Recursive raytracing: " << g_maxShaderRecursion << std::endl;
      break;
    // V key: switch projection mode
    case 'v':
      g_isPerspectiveView = !g_isPerspectiveView;
      if (g_isPerspectiveView) {
        g_view = std::make_unique<PerspectiveView>(g_width, g_height, FOV);
        std::cout << "View mode: Perspective" << std::endl;
      } else {
        g_view = std::make_unique<OrthographicView>(g_width, g_height, ORTHO_VIEW_PLANE_HEIGHT);
        std::cout << "View mode: Orthographic" << std::endl;
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
  // Parse argument
  if (_argc <= 1) {
    std::cerr << "Missing required argument: scene file name" << std::endl;
    return 1;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Initialize GLUT Window
  std::cout << "Initializing GLUTWindow" << std::endl;
  // GLUT
  glutInit(&_argc, _argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(g_width, g_height); // HD size
  g_window = glutCreateWindow("Spiderling: A Rudamentary Game Engine");

  initialize(std::string(_argv[1]));

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
