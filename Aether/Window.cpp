#include "Window.h"
#include<iostream>
#include<GL/glew.h>

Window::Window() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int sdlStatus = SDL_Init(SDL_INIT_EVERYTHING);

    if (sdlStatus < 0) {
        std::cerr << "Error Initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    wnd = SDL_CreateWindow("Aether", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    glContext = SDL_GL_CreateContext(wnd);

    GLenum glewStatus = glewInit();

    if (glewStatus != GLEW_OK) {
        std::cerr << "Error Initializing Glew: " << glewGetErrorString(glewStatus) << std::endl;
        
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(wnd);
        SDL_Quit();
        return;
    }

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Depth Buffer/Culling
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Stencil Buffer
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    isOpen = true;
}

Window::~Window() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(wnd);
    SDL_Quit();
}

void Window::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isOpen = false;
            break;
        }
    }
}

void Window::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::SwapBuffer() {
    SDL_GL_SwapWindow(wnd);
}
