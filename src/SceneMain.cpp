#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>

void SceneMain::init()
{
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
}

void SceneMain::update()
{
}

void SceneMain::render()
{
    SDL_Rect playerRect = { static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height };
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean()
{
    if (player.texture) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
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
