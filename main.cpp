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

    //Test Glider
    cells[((WIDTH / 2) / SQUARE_W) - 1][(HEIGHT / 2) / SQUARE_W].currentState = 1;
    cells[((WIDTH / 2) / SQUARE_W) + 1][((HEIGHT / 2) / SQUARE_W) - 1].currentState = 1;
    cells[(WIDTH / 2) / SQUARE_W][((HEIGHT / 2) / SQUARE_W) + 1].currentState = 1;
    cells[((WIDTH / 2) / SQUARE_W) + 1][(HEIGHT / 2) / SQUARE_W].currentState = 1;
    cells[((WIDTH / 2) / SQUARE_W) + 1][((HEIGHT / 2) / SQUARE_W) + 1].currentState = 1;

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
                const size_t squares_w = WIDTH / SQUARE_W;
                const size_t squares_h = HEIGHT / SQUARE_W;

                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (size_t i = 0; i < squares_w; i++)
                {
                    for (size_t j = 0; j < squares_h; j++)
                    {
                        Cell& current_cell = cells[i % squares_w][j % squares_h];
                        if (mouseX >= current_cell.rect.x && mouseX <= current_cell.rect.x + SQUARE_W &&
                            mouseY >= current_cell.rect.y && mouseY <= current_cell.rect.y + SQUARE_W)
                        {
                            current_cell.currentState = current_cell.currentState != 1;
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
    const size_t squares_w = WIDTH / SQUARE_W;
    const size_t squares_h = HEIGHT / SQUARE_W;
    
    for (size_t x = 0; x < squares_w; x++)
    {
        for (size_t y = 0; y < squares_h; y++)
        {
            size_t livingCells = 0;

            livingCells += cells[(x - 1) % squares_w][(y - 1) % squares_h].currentState;
            livingCells += cells[x % squares_w][(y - 1) % squares_h].currentState;
            livingCells += cells[(x + 1) % squares_w][(y - 1) % squares_h].currentState;
            livingCells += cells[(x - 1) % squares_w][y % squares_h].currentState;
            livingCells += cells[(x + 1) % squares_w][y % squares_h].currentState;
            livingCells += cells[(x - 1) % squares_w][(y + 1) % squares_h].currentState;
            livingCells += cells[x % squares_w][(y + 1) % squares_h].currentState;
            livingCells += cells[(x + 1) % squares_w][(y + 1) % squares_h].currentState;

            Cell& current_cell = cells[x % squares_w][y % squares_h];

            if (current_cell.currentState == 1 && (livingCells > 1 && livingCells < 4))
            {
                current_cell.nextGeneration = 1;
            }
            else if (current_cell.currentState == 0 && livingCells == 3)
            {
                current_cell.nextGeneration = 1;
            }
            else
            {
                current_cell.nextGeneration = 0;
            }
        }
    }
}

void Draw()
{
    const size_t squares_w = WIDTH / SQUARE_W;
    const size_t squares_h = HEIGHT / SQUARE_W;

    for (int x = 0; x < squares_w; x++)
    {
        for (int y = 0; y < squares_h; y++)
        {
            Cell& current_cell = cells[x % squares_w][y % squares_h];
            if (current_cell.currentState == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, R, 0, R, 0);
            }

            SDL_RenderFillRect(renderer, &current_cell.rect);
        }
    }
}

void SetCells()
{
    const size_t squares_w = WIDTH / SQUARE_W;
    const size_t squares_h = HEIGHT / SQUARE_W;

    for (size_t x = 0; x < squares_w; x++)
    {
        for (size_t y = 0; y < squares_h; y++)
        {
            Cell& current_cell = cells[x % squares_w][y % squares_h];
            current_cell.rect.w = current_cell.rect.h = SQUARE_W - 1;
            current_cell.rect.x = x * SQUARE_W;
            current_cell.rect.y = y * SQUARE_W;

            current_cell.currentState = 0;
        }
    }
}

void UpdateCurrent()
{
    const size_t squares_w = WIDTH / SQUARE_W;
    const size_t squares_h = HEIGHT / SQUARE_W;

    for (int x = 0; x < squares_w; x++)
    {
        for (int y = 0; y < squares_h; y++)
        {
            Cell& current_cell = cells[x % squares_w][y % squares_h];
            current_cell.currentState = current_cell.nextGeneration;
            current_cell.nextGeneration = 0;
        }
    }
}