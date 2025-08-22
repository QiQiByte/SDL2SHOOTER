#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::init()
{
}


//游戏的主循环
void Game::run()
{
    while (isRunning) {
        SDL_Event event;
        currentScene->handleEvents(&event); // 处理事件
        currentScene->update(); // 更新场景状态
        currentScene->render(); // 渲染场景
   
        

   
       
    }
 
    
}

void Game::clean()
{
}

void Game::changeScene(Scene* newScene)
{
    if (currentScene) {
        currentScene->clean(); // 清理当前场景资源
        delete currentScene; // 删除当前场景对象，为什么清理后还要删除：
    }
    currentScene = newScene; // 切换到新场景
    if (currentScene) {
        currentScene->init(); // 初始化新场景
    }
}

