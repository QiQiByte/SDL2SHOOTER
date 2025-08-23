#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"
#include "Object.h"

// 前向声明Game类，避免循环依赖，在cpp文件中包含Game.h
class Game;


// 主场景类，继承自Scene
class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    // 初始化场景
    void init() override;

    // 更新场景状态
    void update() override;

    // 渲染场景
    void render() override;

    // 清理场景资源
    void clean() override;

    // 处理事件
    void handleEvents(SDL_Event * event) override;

    void keyboardControls();

private:
    Game &game; // 引用游戏实例
    Player player; // 玩家对象
};


#endif // SCENEMAIN_H