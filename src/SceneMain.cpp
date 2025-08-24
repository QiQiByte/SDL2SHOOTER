#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>
#include <iostream>

void SceneMain::init()
{
    std::random_device rd; // 获取随机数种子
    gen = std::mt19937(rd()); // 初始化随机数生成器
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f); // 均匀分布在0到1之间
    // 使用随机数：
    // float randomValue = dis(gen);


    // 初始化玩家
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (!player.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 3; // 缩小宽度
    player.height /= 3; // 缩小高度

    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化玩家子弹模板
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-2.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 3; // 缩小宽度
    projectilePlayerTemplate.height /= 3; // 缩小高度
    projectilePlayerTemplate.speed = 500;
    if (!projectilePlayerTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load projectile texture: %s", IMG_GetError());
        return; // 注意这里没有return，继续执行 init函数
    }

    // 初始化敌机模板
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 3; // 缩小宽度
    enemyTemplate.height /= 3; // 缩小高度
    enemyTemplate.speed = 100;
    if (!enemyTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy texture: %s", IMG_GetError());
        return;// 注意这里没有return，继续执行 init函数
    }
}

void SceneMain::update(float deltaTime)
{
    //这里的函数每一帧都会调用一次；生成（按键/逻辑）-更新-渲染-清理
    keyboardControls(deltaTime);
    updateProjectilesPlayer(deltaTime);
     spawnEnemy(); // 生成敌人
    updateEnemies(deltaTime);

}

void SceneMain::render()
{
    // 渲染玩家子弹
    renderProjectilesPlayer();

    // 渲染玩家
    SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);

    // 渲染敌人
    renderEnemies();
}

void SceneMain::clean()
{
    // 清理子弹容器
    for (auto projectile : projectilesPlayer) {
        delete projectile;
    }
    projectilesPlayer.clear();
    // 清理敌机容器
    for (auto enemy : enemies) {
        delete enemy;
    }


    enemies.clear();
    // 清理敌机模板资源
    if (enemyTemplate.texture) {
        SDL_DestroyTexture(enemyTemplate.texture);
        enemyTemplate.texture = nullptr;
    }
    // 清理玩家子弹模板资源
    if (projectilePlayerTemplate.texture) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
        projectilePlayerTemplate.texture = nullptr;
    }


    // 清理玩家资源
    if (player.texture) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    // 清理玩家子弹
    for (auto projectile : projectilesPlayer) {
        if (projectile->texture) {
            SDL_DestroyTexture(projectile->texture);
        }
    }

}

void SceneMain::handleEvents(SDL_Event *event)
{
}
SceneMain::SceneMain() : game(Game::getInstance())
{
}
SceneMain::~SceneMain()
{
}
void SceneMain::keyboardControls(float deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(NULL);

    // 使用WASD控制玩家移动
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= deltaTime * player.speed; // 乘以速度因子
        if (player.position.y < 0) player.position.y = 0;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += deltaTime * player.speed;
        if (player.position.y > game.getWindowHeight() - player.height) player.position.y = game.getWindowHeight() - player.height;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= deltaTime * player.speed;
        if (player.position.x < 0) player.position.x = 0;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += deltaTime * player.speed;
        if (player.position.x > game.getWindowWidth() - player.width) player.position.x = game.getWindowWidth() - player.width;
    }

    //玩家使用j键发射子弹
    if (keyboardState[SDL_SCANCODE_J]) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - player.lastShotTime >= player.coolDown) {
            shootPlayer();
            player.lastShotTime = currentTime;
        }
    }

}

void SceneMain::shootPlayer()
{
    ProjectilePlayer* projectile = new ProjectilePlayer(projectilePlayerTemplate); // 使用模板创建新子弹(拷贝构造)
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2; // 子弹从玩家中间位置发射
    projectile->position.y = player.position.y; // 子弹从玩家顶部发射
    projectilesPlayer.push_back(projectile); // 添加到子弹列表
}

void SceneMain::updateProjectilesPlayer(float deltaTime)
{
    int margin = 32; // 子弹超出屏幕边界外的额外距离
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end(); ) {
        ProjectilePlayer* projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime; // 更新子弹位置
        // 如果子弹超出屏幕上方边界，则删除
        if (projectile->position.y < -margin) {
            delete projectile; // 释放内存
            it = projectilesPlayer.erase(it);
            // SDL_Log("player projectile removed");
        }else{
            ++it;
        }

    }
}

void SceneMain::renderProjectilesPlayer()
{
    for (auto projectile : projectilesPlayer) {
        SDL_Rect projectileRect = { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}
void SceneMain::spawnEnemy()
{
    if (dis(gen) > 1 / 60.0f) {
        return; // 以1/60的概率生成敌人
    }
    Enemy* enemy = new Enemy(enemyTemplate); // 使用模板创建新敌人(拷贝构造)
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width); // 随机水平位置
    enemy->position.y = -enemy->height; // 从屏幕上方外侧生成
    enemies.push_back(enemy); // 添加到敌人列表
}
void SceneMain::updateEnemies(float deltaTime)
{
    int margin = 32; // 敌机超出屏幕边界外的额外距离
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Enemy* enemy = *it;
        enemy->position.y += enemy->speed * deltaTime; // 更新敌机位置
        // 如果敌机超出屏幕下方边界，则删除
        if (enemy->position.y > game.getWindowHeight() + margin) {
            delete enemy; // 释放内存
            it = enemies.erase(it);
            // SDL_Log("enemy removed");
        }else{
            ++it;
        }

    }
}
void SceneMain::renderEnemies()
{
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}
