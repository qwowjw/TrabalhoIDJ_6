#ifndef GAME_H
#define GAME_H
#if defined (_WIN64) || defined (_WIN32)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif
#include <string>
#include <iostream>
#include "State.h"
#include "InputManager.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

using namespace std;
class Game {
    static Game* instance;
    SDL_Window* window;
    SDL_Renderer* renderer;
    State* state;
    Game(string title, int width, int height);
    int frameStart;
    float dt;
    void CalculateDeltaTime();
public:
    ~Game();
    State& GetState();
    SDL_Renderer* GetRenderer();
    void Run();
    static Game& GetInstance();
    float GetDeltaTime();
};
#endif