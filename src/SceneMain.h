#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"
#include "Object.h"


// 主场景类，继承自Scene
class SceneMain : public Scene {
public:
    SceneMain() = default;
    ~SceneMain() override = default;

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

private:
    Player player; // 玩家对象
};


#endif // SCENEMAIN_H