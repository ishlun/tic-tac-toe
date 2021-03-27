#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

enum PlayerType
{
    BOT,
    HUMAN
};

enum Cell
{
    X_CELL,
    O_CELL,
    VOID_CELL
};
enum GameState
{
    WIN,
    DRAW,
    IN_PROCESS
};
enum Textures
{
    XO_TEXTURE,
    HELP_TEXTURE,
    XWIN_TEXTURE,
    OWIN_TEXTURE,
    DRAW_TEXTURE,
    TEXTURES_NUM
};
typedef struct pos2d
{
    int x;
    int y;
} pos2d;
typedef struct Game
{
    SDL_Window *window;
    SDL_Texture *textures[TEXTURES_NUM];
    SDL_Renderer *renderer;
    SDL_Rect X_TexPos, O_TexPos;
    int cells[3][3];
    int game_state;
    int winner;
    bool help;
    bool quit;
    bool summon_bot;
    bool game_step;
    pos2d player_pos;
    int player_cell;
    int bot_cell;

} Game;

bool initGame(Game *game_instance);
bool cleanupSDL(Game *game_instance);
bool loadMedia(Game *game_instance);
SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);
void processInput(Game *game_instance);

void gameStep(Game *game_instance);
void playerStep(Game *game_instance);
bool checkGameEnd(Game *game_instance);
bool checkWin(Game *game_instance);
bool checkDraw(Game *game_instance);
void Win(Game *game_instance, int winner);
void Draw(Game *game_instance);
void resetGame(Game *game_instance);

bool isSame(int x, int y, int z);
void switchNums(int *x, int *y);

void drawHelp(Game *game_instance);
void drawDraw(Game *game_instance);
void drawWin(Game *game_instance);
void drawMap(Game *game_instance);
void drawGame(Game *game_instance);


/*
    Чё сделать ёпта
    доделать переключение типов клеток после победы
    и мб починнить функции у бота ебаного блять
*/