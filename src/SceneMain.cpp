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

    // 初始化敌机子弹模板
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 3; // 缩小宽度
    projectileEnemyTemplate.height /= 3; // 缩小高度
    projectileEnemyTemplate.speed = 300;
    if (!projectileEnemyTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy projectile texture: %s", IMG_GetError());
        return;// 注意这里没有return，继续执行 init函数
    }

    // 初始化爆炸模板
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrames = explosionTemplate.width / explosionTemplate.height; // 爆炸图是水平排列的帧
//    explosionTemplate.height /= 3; // 缩小高度
    explosionTemplate.width = explosionTemplate.height; // 保持宽高比1:1
    if (!explosionTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosion texture: %s", IMG_GetError());
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
    updateProjectilesEnemy(deltaTime);
    updatePlayer(deltaTime);
    updateExplosions(deltaTime);


}

void SceneMain::render()
{
    // 渲染玩家子弹
    renderProjectilesPlayer();

    // 渲染玩家
    if (!isGameOver) {
        SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }

    // 渲染敌人
    renderEnemies();

    // 渲染敌人子弹
    renderProjectilesEnemy();

    // 渲染爆炸
    renderExplosions();
}

void SceneMain::clean()
{
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

    // 清理玩家子弹容器
    for (auto projectile : projectilesPlayer) {
        delete projectile;
    }
    projectilesPlayer.clear();

    // 清理玩家子弹模板资源
    if (projectilePlayerTemplate.texture) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
        projectilePlayerTemplate.texture = nullptr;
    }

    //----------------------------------------------------------------

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

    // 清理敌机子弹容器
    for (auto projectile : projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();
    // 清理敌机子弹模板资源
    if (projectileEnemyTemplate.texture) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
        projectileEnemyTemplate.texture = nullptr;
    }

    // 清理爆炸容器
    for (auto explosion : explosions) {
        if (explosion != nullptr) {
            delete explosion;
        }
    }
    explosions.clear();
    // 清理爆炸模板资源
    if (explosionTemplate.texture) {
        SDL_DestroyTexture(explosionTemplate.texture);
        explosionTemplate.texture = nullptr;
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
    if (isGameOver) return; // 游戏结束后不处理输入

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
            bool collisionDetected = false;
            for (auto enemy : enemies ) {
                SDL_Rect enemyRect = { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
                SDL_Rect projectileRect = { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    // 处理碰撞，例如减少敌人生命值，删除子弹等
                    enemy->currentHealth -= projectile->damage;
                    // 删除子弹
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    collisionDetected = true;
                    break;
                }
            }
            if (!collisionDetected) {
                ++it;
            }
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
    if (dis(gen) > 1 / 240.0f) {
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
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Enemy* enemy = *it;
        enemy->position.y += enemy->speed * deltaTime; // 更新敌机位置
        // 如果敌机超出屏幕下方边界，则删除
        if (enemy->position.y > game.getWindowHeight() + margin) {
            delete enemy; // 释放内存
            it = enemies.erase(it);
        }else{
            if (currentTime - enemy->lastShotTime >= enemy->coolDown && !isGameOver) {

                shootEnemy(enemy);

                enemy->lastShotTime = currentTime;
            }
            // 如果敌机生命值小于等于0，则删除
            if (enemy->currentHealth <= 0) {
                explodeEnemy(enemy); // 播放爆炸动画等效果
                it = enemies.erase(it);
            } else{
                ++it;
            }
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
void SceneMain::shootEnemy(Enemy *enemy)
{
    ProjectileEnemy* projectile = new ProjectileEnemy(projectileEnemyTemplate); // 使用模板创建新子弹(拷贝构造)
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2; // 子弹从敌机中间位置发射
    projectile->position.y = enemy->position.y + enemy->height - 5; // 子弹从敌机底部发射
    projectile->direction = getDirection(enemy); // 获取子弹方向
    projectilesEnemy.push_back(projectile); // 添加到子弹列表
}
SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    SDL_FPoint direction;
    float dx = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    float dy = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    float length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        direction.x = dx / length;
        direction.y = dy / length;
    } else {
        direction.x = 0.0f;
        direction.y = 1.0f; // 默认向下
    }
    return direction;
}
void SceneMain::updateProjectilesEnemy(float deltaTime)
{
    int margin = 32; // 子弹超出屏幕边界外的额外距离
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end(); ) {
        ProjectileEnemy* projectile = *it;
        projectile->position.x += projectile->direction.x * projectile->speed * deltaTime; // 更新子弹位置
        projectile->position.y += projectile->direction.y * projectile->speed * deltaTime; // 更新子弹位置
        // 如果子弹超出屏幕边界，则删除
        if (projectile->position.y < -margin || projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.x < -margin || projectile->position.x > game.getWindowWidth() + margin) {
            delete projectile; // 释放内存
            it = projectilesEnemy.erase(it);
            // SDL_Log("enemy projectile removed");
        }else{
            bool collisionDetected = false;
            SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
            SDL_Rect projectileRect = { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
            if (SDL_HasIntersection(&playerRect, &projectileRect) && !isGameOver) {
                // 处理碰撞，例如减少玩家生命值，删除子弹等
                player.currentHealth -= projectile->damage;
                std::cout << "Player hit! Current health: " << player.currentHealth << std::endl;
                // 删除子弹
                delete projectile;
                it = projectilesEnemy.erase(it);
                collisionDetected = true;
            }
            if (!collisionDetected) {
                ++it;
            }
        }

    }

}
void SceneMain::renderProjectilesEnemy()
{
    for (auto projectile : projectilesEnemy) {
        SDL_Rect projectileRect = { static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height };
//        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
        // 计算旋转角度
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180.0f / 3.14159265f - 90.0f; // 加90度使子弹纹理朝上
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}
void SceneMain::explodeEnemy(Enemy *enemy)
{
    auto currentTime = SDL_GetTicks();
    Explosion* explosion = new Explosion(explosionTemplate); // 使用模板创建新爆炸(拷贝构造)
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2; // 爆炸在敌机中间位置
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2; // 爆炸在敌机中间位置
    explosion->startTime = currentTime;
    explosions.push_back(explosion); // 添加到爆炸列表
    // 这里可以添加播放爆炸音效的代码
    delete enemy; // 释放内存
}
void SceneMain::updatePlayer(float deltaTime)
{
    if (isGameOver) return; // 游戏结束后不更新玩家状态

    if (player.currentHealth <= 0) {
        std::cout << "Player destroyed! Game Over!" << std::endl;
        auto currentTime = SDL_GetTicks();
        isGameOver = true;
        // 这里可以添加游戏结束的逻辑，比如切换到游戏结束场景
        auto explosion = new Explosion(explosionTemplate); // 使用模板创建新爆炸(拷贝构造)
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2; // 爆炸在玩家中间位置
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2; // 爆炸在玩家中间位置
        explosion->startTime = currentTime;
        explosions.push_back(explosion); // 添加到爆炸列表
        return;
    }


    for(auto enemy : enemies) {
        SDL_Rect enemyRect = { static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height };
        SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
        if (SDL_HasIntersection(&enemyRect, &playerRect)) {
            // 处理碰撞，例如减少玩家生命值，删除子弹等
            player.currentHealth -= 1;
            enemy->currentHealth = 0; // 碰撞后敌机也被摧毁
            std::cout << "Player hit by enemy! Current health: " << player.currentHealth << std::endl;
        }
    }
}
void SceneMain::updateExplosions(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end(); ) {
        Explosion* explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000; // 计算当前帧
        if (explosion->currentFrame >= explosion->totalFrames) {
            // 爆炸动画结束，删除爆炸对象
            delete explosion;
            it = explosions.erase(it);
        } else {
            ++it;
        }
    }
}
void SceneMain::renderExplosions()
{
    for (auto explosion : explosions) {
        SDL_Rect srcRect = { explosion->currentFrame * explosion->width, 0, explosion->width, explosion->height };
        SDL_Rect destRect = { static_cast<int>(explosion->position.x), static_cast<int>(explosion->position.y), explosion->width, explosion->height };
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &srcRect, &destRect);
    }
}
