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
    int currentHealth = 3;

};

struct ProjectilePlayer
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;

    int damage = 1;
};

struct Enemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 100;

    Uint32 coolDown = 2000; // 冷却时间，单位毫秒
    Uint32 lastShotTime = 0; // 上次射击时间
    int currentHealth = 2;
};

struct ProjectileEnemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 1}; // 默认向下
    int width = 0;
    int height = 0;
    int speed = 500;

    int damage = 1;
};

struct Explosion
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int currentFrame = 0;
    int totalFrames = 0;
    Uint32 startTime = 0;
    int FPS = 10; // 每秒帧数

};



#endif // OBJECT_H