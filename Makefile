CC = gcc
CXX = g++
LD = g++
CFLAGS = `sdl-config --cflags` -g  
LDFLAGS = `sdl-config --libs` -lSDL_image -lGL -lGLU
RM   = /bin/rm -f
OBJS = main.o object.o extra.o game_object.o physics.o obj_shape.o control.o sphere_shape.o gl_renderer.o

all: norbit test_obj

%.o : %.cpp
	$(CXX) $< -o $@ -c -MMD -MF $(basename $@).dep 

%.o : %.c
	$(CC) $< -o $@ -c -MMD -MF $(basename $@).dep 

-include $(wildcard *.dep)

.PHONY: clean

norbit: $(OBJS)
	$(LD) $(LDFLAGS) -o norbit $(OBJS)
test_obj: test_obj.o object.o physics.o
	$(LD) $(LDFLAGS) -o test_obj test_obj.o object.o
test_physics : test_physics.o physics.o
	$(LD) $(LDFLAGS) -o test_physics test_physics.o physics.o

clean:
	$(RM) *~ $(OBJS) norbit test_obj test_obj.o test_physics test_physics.o
	$(RM) *.dep
