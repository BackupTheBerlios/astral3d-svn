GCC=g++
CPPFLAGS=-Wall `sdl-config --cflags`
LDFLAGS=-lGL -lGLU -lm `sdl-config --libs` -lSDL_image -lastral3d

all: level text camera console

clean:
	rm -f *.o level text camera console

level.o:level.cpp
	${GCC} -c -o level.o level.cpp ${CPPFLAGS}

camera.o:camera.cpp
	${GCC} -c -o camera.o camera.cpp ${CPPFLAGS}

text.o:text.cpp
	${GCC} -c -o text.o text.cpp ${CPPFLAGS}

console.o:console.cpp
	${GCC} -c -o console.o console.cpp ${CPPFLAGS}


text: text.o
	${GCC} -o text text.o ${LDFLAGS}

camera: camera.o
	${GCC} -o camera camera.o ${LDFLAGS}

console: console.o
	${GCC} -o console console.o ${LDFLAGS}

level: level.o
	${GCC} -o level level.o ${LDFLAGS}
