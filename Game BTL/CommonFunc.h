#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;

const int FRAMES_PER_SECOND = 25;
const int TILE_SIZE = 64;
const int MAX_MAP_X = 400;
const int MAX_MAP_Y = 10;
const int SPRITE_SIZE = 120;

const int GRAVITY_SPEED = 1;
const int MAX_FALL_SPEED = 10;
const int JUMP_SPEED = 19;
const int DAMAGE_AMOUNT = 5;

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
} Input;

typedef struct Map
{
    int start_x_;
    int start_y_;
    int max_x_;
    int max_y_;
    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;

    void LoadMap(const char* filename);
    void LoadMapTiles(SDL_Renderer* renderer);
    Map GetMap();
    void SetMap(const Map& map);
    void DrawMap(SDL_Renderer* renderer);
} Map;



SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);


void moveObject(SDL_Rect& objRect, int deltaX, int deltaY, int screenWidth, int screenHeight);

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

#endif // COMMONFUNC_H_INCLUDED
#pragma once
