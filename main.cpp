/*
* Conway's Game of life
* By Victor D. Montero
* victordanielmontero@gmail.com
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define SQUARE_W 10

SDL_Window *mainWindow;
SDL_Renderer *renderer;
Uint32 currentTime = 0;
Uint32 lastTime = 0;
Uint32 framePerSeconds = 1;
unsigned char increment = 10;
unsigned char paused = 1;
unsigned char isRunning = 1;
unsigned char R = 0;

typedef struct
{
    SDL_Rect rect;
    unsigned char currentState;
    unsigned char nextGeneration;
} Cell;

Cell cells[WIDTH / SQUARE_W][HEIGHT / SQUARE_W];

void CheckRules();
void Draw();
void UpdateCurrent();
void SetCells();

int main(int args, char **argv)
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Error Initializing: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    atexit(SDL_Quit);

    mainWindow = SDL_CreateWindow(
        "Conway's Game of Life",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        0);

    if (mainWindow == NULL)
    {
        SDL_Log("Error Creating Window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SetCells();

    cells[((WIDTH / 2) / SQUARE_W) - 1][((HEIGHT / 2) / SQUARE_W) - 1].currentState = 1;
    cells[(WIDTH / 2) / SQUARE_W][((HEIGHT / 2) / SQUARE_W) - 1].currentState = 1;
    cells[(WIDTH / 2) / SQUARE_W][(HEIGHT / 2) / SQUARE_W].currentState = 1;
    cells[((WIDTH / 2) / SQUARE_W) + 1][(HEIGHT / 2) / SQUARE_W].currentState = 1;
    cells[(WIDTH / 2) / SQUARE_W][((HEIGHT / 2) / SQUARE_W) + 1].currentState = 1;

    while (isRunning)
    {

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                isRunning = 0;

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    isRunning = 0;

                if (e.key.keysym.scancode == SDL_SCANCODE_P || e.key.keysym.scancode == SDL_SCANCODE_PAUSE)
                    paused = paused == 0 ? 1 : 0;

                if (e.key.keysym.scancode == SDL_SCANCODE_G)
                {
                    R = R == 100 ? 0 : 100;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (unsigned i = 0; i < WIDTH / SQUARE_W; i++)
                {

                    for (unsigned j = 0; j < HEIGHT / SQUARE_W; j++)
                    {
                        if (mouseX >= cells[i][j].rect.x && mouseX <= cells[i][j].rect.x + SQUARE_W &&
                            mouseY >= cells[i][j].rect.y && mouseY <= cells[i][j].rect.y + SQUARE_W)
                        {
                            cells[i][j].currentState = cells[i][j].currentState == 1 ? 0 : 1;
                        }
                    }
                }
            }
        }

        if (paused == 0)
        {
            CheckRules();
            UpdateCurrent();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Draw();
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mainWindow);

    return EXIT_SUCCESS;
}

void CheckRules()
{

    for (int x = 0; x < WIDTH / SQUARE_W; x++)
    {

        for (int y = 0; y < HEIGHT / SQUARE_W; y++)
        {
            int livingCells = 0;

            livingCells += x - 1 < 0 || y - 1 < 0 ? 0 : cells[x - 1][y - 1].currentState;
            livingCells += y - 1 < 0 ? 0 : cells[x][y - 1].currentState;
            livingCells += x + 1 > WIDTH || y - 1 < 0 ? 0 : cells[x + 1][y - 1].currentState;
            livingCells += x - 1 < 0 ? 0 : cells[x - 1][y].currentState;
            livingCells += x + 1 > WIDTH ? 0 : cells[x + 1][y].currentState;
            livingCells += x - 1 < 0 || y + 1 > HEIGHT ? 0 : cells[x - 1][y + 1].currentState;
            livingCells += y + 1 > HEIGHT ? 0 : cells[x][y + 1].currentState;
            livingCells += x + 1 > WIDTH || y + 1 > HEIGHT ? 0 : cells[x + 1][y + 1].currentState;

            if (cells[x][y].currentState == 1 && (livingCells > 1 && livingCells < 4))
            {
                cells[x][y].nextGeneration = 1;
            }
            else if (cells[x][y].currentState == 0 && livingCells == 3)
            {
                cells[x][y].nextGeneration = 1;
            }
            else
            {
                cells[x][y].nextGeneration = 0;
            }
        }
    }
}

void Draw()
{

    for (int x = 0; x < WIDTH / SQUARE_W; x++)
    {
        for (int y = 0; y < HEIGHT / SQUARE_W; y++)
        {
            if (cells[x][y].currentState == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, R, 0, R, 0);
            }

            SDL_RenderFillRect(renderer, &cells[x][y].rect);
        }
    }
}

void SetCells()
{
    for (int x = 0; x < WIDTH / SQUARE_W; x++)
    {
        for (int y = 0; y < HEIGHT / SQUARE_W; y++)
        {
            cells[x][y].rect.w = cells[x][y].rect.h = SQUARE_W - 1;
            cells[x][y].rect.x = x * SQUARE_W;
            cells[x][y].rect.y = y * SQUARE_W;

            cells[x][y].currentState = 0;
        }
    }
}

void UpdateCurrent()
{

    for (int x = 0; x < WIDTH / SQUARE_W; x++)
    {
        for (int y = 0; y < HEIGHT / SQUARE_W; y++)
        {
            cells[x][y].currentState = cells[x][y].nextGeneration;
            cells[x][y].nextGeneration = 0;
        }
    }
}