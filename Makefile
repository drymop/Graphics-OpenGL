################################################################################
## OS
################################################################################
OS = LINUX
#OS = OSX

################################################################################
## GCC
################################################################################
CC = g++ -std=c++17
OPTS = -O3
#OPTS = -g
FLAGS = -Wall -Werror
ifeq "$(OS)" "LINUX"
  DEFS = -DLINUX
else
  ifeq "$(OS)" "OSX"
  DEFS = -DOSX
endif
endif

################################################################################
## GL and QT
################################################################################
# Open gl
ifeq "$(OS)" "LINUX"
  GL_LIBS = -lglut -lGL
else
  ifeq "$(OS)" "OSX"
  GL_LIBS = -framework GLUT -framework OpenGL
endif
endif

################################################################################
## Rules
################################################################################
INCL = $(GL_INCL)
LIBS = $(GL_LIBS) -lpthread

# OBJS for ray tracer
OBJS = \
       mainRayTracer.o \
       Circle.o \
       CompileShaders.o \
       ConfigParser.o \
       ObjFileParser.o \
       OrthographicView.o \
       PerspectiveView.o \
       Plane.o \
       PointLight.o \
       Portal.o \
       RasterizableObject.o \
       Rasterizer.o \
       RayTracer.o \
       Scene.o \
       SceneBuilder.o \
       Sphere.o \

# # OBJS for rasterizer
# OBJS = \
#        mainRasterizer.o \
#        CompileShaders.o \
#        ObjFileParser.o \
#        RasterizableObject.o \

EXECUTABLE = main.out

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(OBJMOC)
	$(CC) $(OPTS) $(FLAGS) $(DEFS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) Dependencies $(OBJS)

.cpp.o:
	$(CC) $(OPTS) $(DEFS) -MMD $(INCL) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d

-include Dependencies