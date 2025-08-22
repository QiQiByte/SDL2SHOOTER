#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL.h>

class Game{
public:
    Game();
    ~Game();

    void init();
    void run();
    void clean();
    void changeScene(Scene* newScene);

private:
    bool isRunning = true;
    Scene* currentScene = nullptr; // 当前场景指针
    
};

#endif