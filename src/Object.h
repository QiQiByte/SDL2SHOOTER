#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

struct Player
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 300;

    Uint32 coolDown = 100; // 冷却时间，单位毫秒
    Uint32 lastShotTime = 0; // 上次射击时间

};

struct ProjectilePlayer
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
};



#endif // OBJECT_H