#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
    clean();
}



void Game::init()
{
    frameTime = 1000 / FPS; // 每帧的时间，单位毫秒

    //SDL初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        // 初始化失败，处理错误
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Unable to initialize SDL: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    //创建窗口
    window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    //初始化SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL_image: %s", IMG_GetError());
        isRunning = false;
        return;
    }


    isRunning = true;
    currentScene = new SceneMain(); // 创建主场景实例
    currentScene->init(); // 初始化主场景

}


//游戏的主循环
void Game::run()
{
    while (isRunning) {
        Uint32 startTime = SDL_GetTicks(); // 获取当前时间，单位毫秒

        SDL_Event event;
        handleEvents(&event); // 处理事件
        update(deltaTime); // 更新游戏状态
        render(); // 渲染当前场景

        Uint32 elapsedTime = SDL_GetTicks() - startTime; // 计算本帧耗时
        if (elapsedTime < frameTime) {
            SDL_Delay(frameTime - elapsedTime); // 延时以维持稳定帧率
            deltaTime = frameTime / 1000.0f; // 计算每帧的时间，单位秒
        }
        deltaTime = (SDL_GetTicks() - startTime) / 1000.0f; // 计算每帧的时间，单位秒
    }
 
    
}

void Game::clean()
{
    if (currentScene) {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
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

void Game::handleEvents(SDL_Event* event)
{
    while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT) {
                isRunning = false; // 处理退出事件
            }
            if (currentScene) {
                currentScene->handleEvents(event); // 传递事件给当前场景处理
            }
        currentScene->handleEvents(event); // 处理事件
        }
}

void Game::update(float deltaTime)
{
    if (currentScene) {
        currentScene->update(deltaTime);
    }
}   

void Game::render()
{
    SDL_RenderClear(renderer); // 清除屏幕
    currentScene->render(); // 渲染场景
    SDL_RenderPresent(renderer); // 显示渲染结果
}

