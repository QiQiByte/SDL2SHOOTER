#ifndef SCENE_H
#define SCENE_H

//场景类基类

#include <SDL.h>

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default; // 虚析构函数，确保派生类的析构函数被调用，不然不会被调用

    // 初始化场景
    virtual void init() = 0;

    // 更新场景状态
    virtual void update() = 0;

    // 渲染场景
    virtual void render() = 0;

    // 清理场景资源
    virtual void clean() = 0;

    //处理事件
    virtual void handleEvents(SDL_Event * event) = 0;
};




#endif