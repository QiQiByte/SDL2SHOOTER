#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>
#include <iostream>

void SceneMain::init()
{
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
}

void SceneMain::update(float deltaTime)
{
    keyboardControls(deltaTime);
    updateProjectilesPlayer(deltaTime);
}

void SceneMain::render()
{
    // 渲染玩家子弹
    renderProjectilesPlayer();



    // 渲染玩家
    SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean()
{
    // 清理容器
    for (auto projectile : projectilesPlayer) {
        delete projectile;
    }
    projectilesPlayer.clear();


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
