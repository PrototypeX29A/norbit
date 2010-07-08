CC = g++
LD = g++
CFLAGS = `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image -lGL
RM   = /bin/rm -f
OBJS = main.o object.o extra.o shadow.o game_object.o physics.o


.PHONY: clean
all: norbit test_obj
norbit: $(OBJS)
	$(LD) $(LDFLAGS) -o norbit $(OBJS)
test_obj: test_obj.o object.o physics.o
	$(LD) $(LDFLAGS) -o test_obj test_obj.o object.o
test_physics : test_physics.o physics.o
	$(LD) $(LDFLAGS) -o test_physics test_physics.o physics.o
main.o: main.c extra.h object.h shadow.h
	$(CC) $(CFLAGS) -c main.c
object.o: object.c object.h
	$(CC) $(CFLAGS) -c object.c
extra.o: extra.c extra.h
	$(CC) $(CFLAGS) -c extra.c
shadow.o: shadow.c shadow.h
	$(CC) $(CFLAGS) -c shadow.c
test_obj.o: test_obj.c object.h
	$(CC) $(CFLAGS) -c test_obj.c
test_physics.o: test_physics.cpp physics.h 
	$(CC) $(CFLAGS) -c test_physics.cpp
physics.o: physics.cpp physics.h math2d.h
	$(CC) $(CFLAGS) -c physics.cpp
game_object.o: physics.h math2d.h shadow.h object.h 
	$(CC) $(CFLAGS) -c game_object.cpp
clean:
	$(RM) *~ $(OBJS) norbit test_obj test_obj.o test_physics test_physics.o
