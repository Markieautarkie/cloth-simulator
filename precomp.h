// add your includes to this file instead of to individual .cpp files
// to enjoy the benefits of precompiled headers:
// - fast compilation
// - solve issues with the order of header files once (here)
// do not include headers in header files (ever)

// constants
#define PI 3.1415926535897932384626433832795

#define MOUSESENSITIVITY 0.75f        // sensitivity of the mouse
#define SCRWIDTH 1280                 // the width of the application window
#define SCRHEIGHT 720                 // the height of the application window

#define DAMPING 0.01                  // the amount of damping on mass spring systems
#define TIMESTEP 0.5                  // basic timestep
#define TIMESTEP2 TIMESTEP * TIMESTEP // integrated timestep

// enum for cloth patterns
enum class Pattern { Vertical, Horizontal, Checkerboard, Random};

// basic includes for OpenGL
#include "glad/glad.h"
#include <GLFW/glfw3.h>

// helpers
#include <iostream>
#include <ctime>
#include <vector>
#include "vec3.h"
#include "camera.h"
#include "openglhelper.h"

// headers
#include "particle.h"
#include "constraint.h"
#include "cloth.h"
#include "sphere.h"