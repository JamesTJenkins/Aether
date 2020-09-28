#pragma once
#include <SDL2/SDL.h>
#undef main

class Window {
public:
	Window();
	~Window();

	inline bool IsOpen() const {
		return isOpen;
	}

	void HandleEvents();
	void Clear(float r, float g, float b, float a);
	void SwapBuffer();
private:
	SDL_Window* wnd;
	SDL_GLContext glContext;
	bool isOpen;
};

