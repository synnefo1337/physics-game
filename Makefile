main: main.cpp
	g++ main.cpp -ID:\SDL_Library\include\SDL2 -LD:\SDL_Library\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o main.exe

clean:
	rm main.exe