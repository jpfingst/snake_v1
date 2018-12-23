GSL_CFLAGS = -I/opt/local/include
SDL_CFLAGS = -I../SDL2-2.0.8/include -I../SDL2_image-2.0.3 -I../SDL2_ttf-2.0.14 -I../SDL2_mixer-2.0.2
GSL_LDFLAGS = -L/opt/local/lib -lgsl
SDL_LDFLAGS = -L/opt/local/lib -lSDL2 -L/usr/local/lib -lSDL2_image -lSDL2_ttf -lSDL2_mixer

CXX = g++
CXXFLAGS = -I../include $(GSL_CFLAGS) $(SDL_CFLAGS)
LDFLAGS = $(GSL_LDFLAGS) $(SDL_LDFLAGS)


OBJ = main.o texture.o tile.o snake.o mouse.o feeder.o score.o link.o

snake: $(OBJ)
	$(CXX) $(LDFLAGS) -o snake $(OBJ)

%.o: %c
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm $(OBJ)
	rm game_test
