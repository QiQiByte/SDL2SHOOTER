#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Game{
public:
    Game();
    ~Game();

    void init();
    void run();
    void clean();
    void changeScene(Scene* newScene);

    void handleEvents(SDL_Event* event);
    void update();
    void render();

private:
    bool isRunning = true;
    Scene* currentScene = nullptr; // 当前场景指针
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const int windowWidth = 600;
    const int windowHeight = 800;
    
};

#endif