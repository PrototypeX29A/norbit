CC = gcc
CXX = g++
LD = g++
CFLAGS = `sdl-config --cflags` -g  `pkg-config --libs --cflags libzmq`  -g   
LDFLAGS = `sdl-config --libs` -lSDL_image -lGL -lGLU `pkg-config --libs --cflags libzmq` -lprotobuf -lpthread
RM   = /bin/rm -f
OBJS =  game_object.o physics.o control.o $(addprefix rendering/,$(RNDR_OBJS))
RNDR_OBJS = object.o extra.o sphere_shape.o  gl_renderer.o gl_drawable.o obj_shape.o \
desktop_camera.o ego_camera.o
NET_OBJS = net/norbit.pb.o net/netcontrol.o

all: norbit test_obj norbit_client norbit_server

%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@ -c -MMD -MF $(basename $@).dep 

%.o : %.cc 
	$(CXX) $(CFLAGS) -I. $< -o $@ -c -MMD -MF $(basename $@).dep 

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@ -c -MMD -MF $(basename $@).dep 

%.pb.h : %.proto
	protoc $< --cpp_out=.

%.pb.cc : %.proto
	protoc $< --cpp_out=.

-include $(wildcard *.dep)

.PHONY: clean

norbit: $(OBJS) main.o
	$(LD) $(LDFLAGS) -o norbit $(OBJS) main.o
norbit_client: $(OBJS) $(NET_OBJS) main_client.o net/norbit.pb.h 
	$(LD) $(LDFLAGS) -o norbit_client $(OBJS) $(NET_OBJS) main_client.o
norbit_server: $(OBJS) $(NET_OBJS) main_server.o net/norbit.pb.h
	$(LD) $(LDFLAGS) -o norbit_server $(OBJS) $(NET_OBJS) main_server.o
test_obj: test_obj.o rendering/object.o physics.o 
	$(LD) $(LDFLAGS) -o test_obj test_obj.o rendering/object.o
test_physics : test_physics.o physics.o
	$(LD) $(LDFLAGS) -o test_physics test_physics.o physics.o

clean:
	$(RM) *~ $(OBJS) $(RNDR_OBJS) $(NET_OBJS) norbit test_obj test_obj.o test_physics test_physics.o  norbit_client norbit_server
	$(RM) net/*.pb.*
	$(RM) *.dep net/*.dep rendering/*.dep
