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
    //单例模式：③提供一个静态方法获取唯一实例
    static Game& getInstance(){
        static Game instance; // 局部静态变量，确保只创建一次实例
        return instance;
    };
    ~Game();

    void init();
    void run();
    void clean();

    void changeScene(Scene* newScene);

    void handleEvents(SDL_Event* event);
    void update(float deltaTime);
    void render();

    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }


private:
    // 单例模式：①构造函数私有化，防止构造多个
    Game();
    //单例模式：②删除拷贝构造函数和赋值运算符，防止拷贝
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool isRunning = true;
    Scene* currentScene = nullptr; // 当前场景指针，基类指针，后面可以指向不同的场景的子类对象
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const int windowWidth = 600;
    const int windowHeight = 800;

    int FPS = 120; // 目标帧率
    Uint32 frameTime; // 每帧的时间，单位毫秒
    float deltaTime; // 每帧的时间，单位秒

};

#endif