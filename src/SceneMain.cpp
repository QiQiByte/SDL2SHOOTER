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
    keyboardControls();
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
void SceneMain::keyboardControls()
{
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= 0.1;
        if (player.position.y < 0) player.position.y = 0;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += 0.1;
        if (player.position.y > game.getWindowHeight() - player.height) player.position.y = game.getWindowHeight() - player.height;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= 0.1;
        if (player.position.x < 0) player.position.x = 0;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += 0.1;
        if (player.position.x > game.getWindowWidth() - player.width) player.position.x = game.getWindowWidth() - player.width;
    }
}
