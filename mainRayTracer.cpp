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
#include <GL/freeglut.h>

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "Camera.h"
#include "ConfigParser.h"
#include "LightSource.h"
#include "Material.h"
#include "OrthographicView.h"
#include "PerspectiveView.h"
#include "Plane.h"
#include "PointLight.h"
#include "Scene.h"
#include "SceneBuilder.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "RayTracableObject.h"
#include "View.h"

using glm::vec2, glm::vec3, glm::vec4, glm::mat4;

////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these

// Window
int g_width{1280};
int g_height{720};
int g_window{0};

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

// Define view
bool g_isPerspectiveView{true};
// Anti-aliasing
bool g_hasAntiAliasing{false};

// Scene to render
Scene g_scene;

// Ray tracer
RayTracer g_rayTracer{g_width, g_height};
const int N_RAY_TRACE_RECURSIONS = 5;
int g_maxRayTracerRecursion{0};

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize settings
void
initialize(const std::string& sceneFile) {
  glClearColor(0.f, 0.f, 0.f, 0.f);

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
  // renderer
  g_rayTracer.setFrameSize(g_width, g_height);
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
/// @brief Draw function for single frame
void
draw() {
  using namespace std::chrono;

  //////////////////////////////////////////////////////////////////////////////
  // Draw
  g_rayTracer.render(g_scene);

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
      g_hasAntiAliasing = !g_rayTracer.hasAntiAlias();
      g_rayTracer.setAntiAlias(g_hasAntiAliasing);
      std::cout << "Anti-alias: " << g_hasAntiAliasing << std::endl;
      break;
    // V key: switch projection mode
    case 'v':
      g_isPerspectiveView = !g_rayTracer.isPerspectiveView();
      g_rayTracer.setPerspectiveView(g_isPerspectiveView);
      if (g_isPerspectiveView) {
        std::cout << "View mode: Perspective" << std::endl;
      } else {
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

const int GLUT_NO_BUTTON = -1;
const int GLUT_WHEEL_UP = 3;
const int GLUT_WHEEL_DOWN = 4;

// only 1 mouse button can be active, other will be ignored
int g_activeDragButton = GLUT_NO_BUTTON;
int g_mouseStartX;
int g_mouseStartY;
vec3 g_eyeStart;
float g_scale = 0.02f;
float g_zScale = 0.2f;

vec3 g_atStart, g_upStart, g_rightStart;
float g_angleDelta = 0.005f;

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for mouse presses
void 
mousePressed(int button, int state, int x, int y) {
  std::cout << "Button " << button << " active " << g_activeDragButton << std::endl;
  if (button == g_activeDragButton) {
    // release the active button
    g_activeDragButton = GLUT_NO_BUTTON;
    return;
  } else if (g_activeDragButton != GLUT_NO_BUTTON) {
    // ignore other button if there's already an active button
    return;
  }
  Camera& camera = g_scene.getCamera();
  switch(button) {
    case GLUT_LEFT_BUTTON:
      g_activeDragButton = GLUT_LEFT_BUTTON;
      g_mouseStartX = x;
      g_mouseStartY = y;
      g_eyeStart = camera.getEye();
      break;
    case GLUT_RIGHT_BUTTON:
      g_activeDragButton = GLUT_RIGHT_BUTTON;
      g_mouseStartX = x;
      g_mouseStartY = y;
      g_atStart = camera.getAt();
      g_upStart = camera.getUp();
      g_rightStart = camera.getRight();
      break;
    case GLUT_WHEEL_UP:
      camera.setPosition(camera.getEye() + camera.getAt() * g_zScale);
      break;
    case GLUT_WHEEL_DOWN:
      camera.setPosition(camera.getEye() - camera.getAt() * g_zScale);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for mouse actions
void
mouseDragged(int x, int y) {
  Camera& camera = g_scene.getCamera();
  if(g_activeDragButton == GLUT_LEFT_BUTTON) {
    float dx = x - g_mouseStartX;
    float dy = y - g_mouseStartY;
    // change camera position
    vec3 eye = g_eyeStart + (camera.getRight() * -dx + camera.getUp() * dy) * g_scale;
    camera.setPosition(eye);
  } else if(g_activeDragButton == GLUT_RIGHT_BUTTON) {
    float dx = x - g_mouseStartX;
    float dy = g_mouseStartY - y;
    vec2 delta(-dy, dx);
    float d = glm::length(delta);
    delta = glm::normalize(delta);
    
    std::cout << dx << " " << dy << " "<< d << std::endl;

    vec3 rotationAxis = g_rightStart * delta.x + g_upStart * delta.y;
    float rotationAngle = d * g_angleDelta;
    mat4 rotationMatrix = glm::rotate(mat4(1.f), rotationAngle, rotationAxis);
    vec3 at = rotationMatrix * vec4(g_atStart, 1.f);
    vec3 up = rotationMatrix * vec4(g_upStart, 1.f);
    camera.setOrientation(at, up);
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
  glutInit(&_argc, _argv);
  if (_argc <= 1) {
    std::cerr << "Missing required argument: config file name" << std::endl;
    return 1;
  }
  ConfigParser configParser;
  Config config = configParser.parse(_argv[1]);
  g_width = config.screenWidth;
  g_height = config.screenHeight;

  //////////////////////////////////////////////////////////////////////////////
  // Initialize GLUT Window
  std::cout << "Initializing GLUTWindow" << std::endl;
  // GLUT
  if (!config.rayTracing) {
    // rasterizing
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
  }
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(g_width, g_height); // HD size
  g_window = glutCreateWindow("Spiderling: A Rudamentary Game Engine");

  initialize(config.sceneFile);

  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glutReshapeFunc(resize);
  glutDisplayFunc(draw);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutMouseFunc(mousePressed);
  glutMotionFunc(mouseDragged);
  glutPassiveMotionFunc(NULL);
  glutTimerFunc(1000/FPS, timer, 0);

  // Start application
  std::cout << "Starting Application" << std::endl;
  glutMainLoop();

  return 0;
}

#if   defined(OSX)
#pragma clang diagnostic pop
#endif
