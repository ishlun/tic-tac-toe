#include "main.h"
#include "bot.h"

bool initGame(Game *game_instance)
{
    freopen("/dev/stdout", "w", stdout);

    srand(time(NULL));

    //init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Can't init SDL. SDL error: %s\n", SDL_GetError());
        return false;
    }

    //init SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) | IMG_INIT_PNG))
    {
        printf("Can't init SDL_image. SDL error: %s\n", SDL_GetError());
        return false;
    }

    //create window
    game_instance->window = SDL_CreateWindow("Крестики-Нолики ебать нахуй блять",
                                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
    if (game_instance->window == NULL)
    {
        printf("Can't create window. SDL error: %s\n", SDL_GetError());
        return false;
    }

    //create renderer
    game_instance->renderer = SDL_CreateRenderer(game_instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game_instance->renderer == NULL)
    {
        printf("Can't create renderer. SDL error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(game_instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    game_instance->X_TexPos.x = 0;
    game_instance->X_TexPos.y = 0;
    game_instance->X_TexPos.w = 64;
    game_instance->X_TexPos.h = 64;

    game_instance->O_TexPos.x = 64;
    game_instance->O_TexPos.y = 0;
    game_instance->O_TexPos.w = 128;
    game_instance->O_TexPos.h = 64;

    return true;
}
bool cleanupSDL(Game *game_instance)
{
    for (int i = 0; i < TEXTURES_NUM - 1; i++)
        SDL_DestroyTexture(game_instance->textures[i]);

    IMG_Quit();
    SDL_Quit();
}
bool loadMedia(Game *game_instance)
{
    game_instance->textures[XO_TEXTURE] = loadTexture("assets/XO.png", game_instance->renderer);
    if (game_instance->textures[XO_TEXTURE] == NULL)
        return false;

    game_instance->textures[HELP_TEXTURE] = loadTexture("assets/help.png", game_instance->renderer);
    if (game_instance->textures[HELP_TEXTURE] == NULL)
        return false;

    game_instance->textures[OWIN_TEXTURE] = loadTexture("assets/lose.png", game_instance->renderer);
    if (game_instance->textures[OWIN_TEXTURE] == NULL)
        return false;

    game_instance->textures[XWIN_TEXTURE] = loadTexture("assets/win.png", game_instance->renderer);
    if (game_instance->textures[XWIN_TEXTURE] == NULL)
        return false;

    game_instance->textures[DRAW_TEXTURE] = loadTexture("assets/draw.png", game_instance->renderer);
    if (game_instance->textures[DRAW_TEXTURE] == NULL)
        return false;

    return true;
}
SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Surface *image_surface = NULL;
    SDL_Texture *texture = NULL;

    image_surface = IMG_Load(path);
    if (image_surface == NULL)
    {
        printf("Can't load %s. SDL error: %s\n", path, SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    if (texture == NULL)
    {
        printf("Can't create texture from %s. SDL error: %s\n", path, SDL_GetError());
        return NULL;
    }

    return texture;
}
void processInput(Game *game_instance)
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
            game_instance->quit = true;

        if (event.type == SDL_KEYDOWN)
        {
            //if help menu opened we ignore any input except P
            if (!game_instance->help && (game_instance->game_state == IN_PROCESS))
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    game_instance->player_pos = (pos2d){0, 0};
                    game_instance->game_step = true;
                    break;
                case SDLK_w:
                    game_instance->player_pos = (pos2d){1, 0};
                    game_instance->game_step = true;
                    break;
                case SDLK_e:
                    game_instance->player_pos = (pos2d){2, 0};
                    game_instance->game_step = true;
                    break;
                case SDLK_a:
                    game_instance->player_pos = (pos2d){0, 1};
                    game_instance->game_step = true;
                    break;
                case SDLK_s:
                    game_instance->player_pos = (pos2d){1, 1};
                    game_instance->game_step = true;
                    break;
                case SDLK_d:
                    game_instance->player_pos = (pos2d){2, 1};
                    game_instance->game_step = true;
                    break;
                case SDLK_z:
                    game_instance->player_pos = (pos2d){0, 2};
                    game_instance->game_step = true;
                    break;
                case SDLK_x:
                    game_instance->player_pos = (pos2d){1, 2};
                    game_instance->game_step = true;
                    break;
                case SDLK_c:
                    game_instance->player_pos = (pos2d){2, 2};
                    game_instance->game_step = true;
                    break;
                default:
                    break;
                }
            }

            if (event.key.keysym.sym == SDLK_p)
                game_instance->help = !game_instance->help;
            if ((game_instance->game_state == WIN || game_instance->game_state == DRAW) && event.key.keysym.sym == SDLK_r)
            {
                resetGame(game_instance);
            }
        }
    }
}

void gameStep(Game *game_instance)
{
    if (game_instance->game_step && game_instance->player_cell == X_CELL)
    {
        playerStep(game_instance);
        game_instance->game_step = false;
        printf("player moves\n");

        if (checkGameEnd(game_instance))
        {
            game_instance->summon_bot = false;
            return;
        }
    }
    else if (game_instance->summon_bot && game_instance->bot_cell == X_CELL)
    {
        pos2d pos = botStep(game_instance->cells, game_instance->bot_cell);
        game_instance->cells[pos.x][pos.y] = game_instance->bot_cell;
        game_instance->summon_bot = false;
        printf("bot moves\n");
        if (checkGameEnd(game_instance))
            return;
    }

    if (game_instance->game_step && game_instance->player_cell == O_CELL)
    {
        playerStep(game_instance);
        game_instance->game_step = false;
        printf("player moves\n");

        if (checkGameEnd(game_instance))
        {
            game_instance->summon_bot = false;
            return;
        }
    }
    else if (game_instance->summon_bot && game_instance->bot_cell == O_CELL)
    {
        pos2d pos = botStep(game_instance->cells, game_instance->bot_cell);
        game_instance->cells[pos.x][pos.y] = game_instance->bot_cell;
        game_instance->summon_bot = false;
        printf("bot moves\n");
        if (checkGameEnd(game_instance))
            return;
    }
}
void playerStep(Game *game_instance)
{

    int x = game_instance->player_pos.x;
    int y = game_instance->player_pos.y;

    if (game_instance->cells[x][y] != VOID_CELL)
        return;

    game_instance->summon_bot = true;
    game_instance->cells[x][y] = game_instance->player_cell;
}
bool checkGameEnd(Game *game_instance)
{
    if (checkWin(game_instance))
        return true;
    if (checkDraw(game_instance))
        return true;
    return false;
}
bool checkWin(Game *game_instance)
{
    for (int i = 0; i < 3; i++)
    {
        //vertical line
        if (isSame(game_instance->cells[i][0], game_instance->cells[i][1], game_instance->cells[i][2]) && (game_instance->cells[i][0] != VOID_CELL))
        {
            Win(game_instance, game_instance->cells[i][0]);
            return true;
        }

        //horizontal line
        if (isSame(game_instance->cells[0][i], game_instance->cells[1][i], game_instance->cells[2][i]) && (game_instance->cells[0][i] != VOID_CELL))
        {
            Win(game_instance, game_instance->cells[0][i]);
            return true;
        }
    }

    if (isSame(game_instance->cells[0][0], game_instance->cells[1][1], game_instance->cells[2][2]) && (game_instance->cells[0][0] != VOID_CELL))
    {
        Win(game_instance, game_instance->cells[0][0]);
        return true;
    }
    if (isSame(game_instance->cells[2][0], game_instance->cells[1][1], game_instance->cells[0][2]) && (game_instance->cells[2][0] != VOID_CELL))
    {
        Win(game_instance, game_instance->cells[2][0]);
        return true;
    }
    return false;
}
bool checkDraw(Game *game_instance)
{
    int void_num = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (game_instance->cells[i][j] == VOID_CELL)
                void_num++;

    if (void_num == 0)
    {
        Draw(game_instance);
        return true;
    }
    return false;
}
void Win(Game *game_instance, int winner)
{
    game_instance->game_state = WIN;
    game_instance->winner = (winner == game_instance->bot_cell ? HUMAN : BOT);
    switchNums(&game_instance->player_cell, &game_instance->bot_cell);
}
void Draw(Game *game_instance)
{
    game_instance->game_state = DRAW;
    switchNums(&game_instance->player_cell, &game_instance->bot_cell);
}
void resetGame(Game *game_instance)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            game_instance->cells[i][j] = VOID_CELL;

    game_instance->game_state = IN_PROCESS;
    game_instance->summon_bot = (game_instance->bot_cell == X_CELL);
    game_instance->game_step = false;
}

bool isSame(int x, int y, int z)
{
    if ((x == y) && (y == z) && (x == z))
        return true;
    return false;
}
void switchNums(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

void drawGame(Game *game_instance)
{
    if (game_instance->help)
    {
        drawHelp(game_instance);
    }
    else if (game_instance->game_state == WIN)
    {
        drawWin(game_instance);
    }
    else if (game_instance->game_state == DRAW)
    {
        drawDraw(game_instance);
    }
    else
    {
        drawMap(game_instance);
    }
}
void drawHelp(Game *game_instance)
{
    SDL_RenderCopy(game_instance->renderer, game_instance->textures[HELP_TEXTURE], NULL, NULL);
}
void drawDraw(Game *game_instance)
{
    SDL_RenderCopy(game_instance->renderer, game_instance->textures[DRAW_TEXTURE], NULL, NULL);
}
void drawWin(Game *game_instance)
{

    if (game_instance->winner == BOT)
        SDL_RenderCopy(game_instance->renderer, game_instance->textures[XWIN_TEXTURE], NULL, NULL);
    else if (game_instance->winner == HUMAN)
        SDL_RenderCopy(game_instance->renderer, game_instance->textures[OWIN_TEXTURE], NULL, NULL);
}
void drawMap(Game *game_instance)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            SDL_Rect screenPos;
            screenPos.x = (SCREEN_WIDTH / 3) * i;
            screenPos.y = (SCREEN_HEIGHT / 3) * j;
            screenPos.w = (SCREEN_WIDTH / 3);
            screenPos.h = (SCREEN_HEIGHT / 3);

            switch (game_instance->cells[i][j])
            {
            case X_CELL:
                SDL_RenderCopy(game_instance->renderer, game_instance->textures[XO_TEXTURE], &game_instance->X_TexPos, &screenPos);
                break;
            case O_CELL:
                SDL_RenderCopy(game_instance->renderer, game_instance->textures[XO_TEXTURE], &game_instance->O_TexPos, &screenPos);
                break;
            default:
                break;
            }
        }
    }
}

int main()
{
    printf("Крестики-Нолики нахуй 0.1.2\n");
    Game game_instance;
    game_instance.player_cell = X_CELL;
    game_instance.bot_cell = O_CELL;
    resetGame(&game_instance);

    if (!initGame(&game_instance))
        return -1;
    if (!loadMedia(&game_instance))
        return -1;

    game_instance.quit = false;
    game_instance.help = true;

    while (!game_instance.quit)
    {
        processInput(&game_instance);

        gameStep(&game_instance);

        SDL_RenderClear(game_instance.renderer);
        drawGame(&game_instance);
        SDL_RenderPresent(game_instance.renderer);
    }

    cleanupSDL(&game_instance);
    return 0;
}