#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>


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
    void update(float deltaTime) override;
    // 渲染场景
    void render() override;
    // 清理场景资源
    void clean() override;
    // 处理事件
    void handleEvents(SDL_Event * event) override;

    void keyboardControls(float deltaTime);
    void shootPlayer();
    void updateProjectilesPlayer(float deltaTime);
    void renderProjectilesPlayer();

    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void shootEnemy(Enemy* enemy);
    SDL_FPoint getDirection(Enemy* enemy);
    void updateProjectilesEnemy(float deltaTime);
    void renderProjectilesEnemy();

private:
    Game &game; // 引用游戏实例
    Player player; // 玩家对象

    //创建玩家子弹模板
    ProjectilePlayer projectilePlayerTemplate;
    //创建敌机模板
    Enemy enemyTemplate;
    //创建敌机子弹模板
    ProjectileEnemy projectileEnemyTemplate;


    //创建玩家子弹容器（列表）
    std::list<ProjectilePlayer*> projectilesPlayer; // 玩家子弹列表, 使用指针方便管理,防止堆栈数据过多
    //创建敌机容器（列表）
    std::list<Enemy*> enemies;
    //创建敌机子弹容器（列表）
    std::list<ProjectileEnemy*> projectilesEnemy;

    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器（均匀分布）

};


#endif // SCENEMAIN_H