#include <stdio.h>
#include <SDL.h>

constexpr int dim = 900;
constexpr int border = 50;
constexpr int gridSize = 9;
constexpr int offset = (dim - border * 2) / (gridSize - 1);
constexpr int stoneRadius = offset / 2 - 5;

int player = 1;
int grid[gridSize][gridSize] = {0};
SDL_Point gridCoords[gridSize][gridSize];
SDL_Window *win;
SDL_Renderer *ren;

void preComputeGridCoordinates() {
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            SDL_Point p;
            p.x = border + x * offset;
            p.y = border + y * offset;
            gridCoords[x][y] = p;
        }
    }
}

void drawBackground() {
    SDL_SetRenderDrawColor(ren, 194, 136, 78, 255);
    SDL_RenderClear(ren);
}

void drawGrid() {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    for (int i = 0; i < gridSize; ++i) {
        SDL_RenderDrawLine(ren, gridCoords[i][0].x, border, gridCoords[i][gridSize - 1].x, dim - border); //Vertical
        SDL_RenderDrawLine(ren, border, gridCoords[0][i].y, dim - border, gridCoords[gridSize - 1][i].y); //Horizontal
    }
}

void drawStone(SDL_Point p) {
    if (player == 1)
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    else if (player == -1)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    for (int x = p.x - stoneRadius; x <= p.x + stoneRadius; x++)
        for (int y = p.y - stoneRadius; y <= p.y + stoneRadius; y++) {
            int dx = x - p.x;
            int dy = y - p.y;
            if (dx*dx + dy*dy <= stoneRadius*stoneRadius)
                SDL_RenderDrawPoint(ren, x, y);
        }
}

void drawStones() {

}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Go",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dim, dim, SDL_WINDOW_RESIZABLE);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    preComputeGridCoordinates();

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                quit = 1;
        }

        drawBackground();
        drawGrid();
        player = 1;
        drawStone(gridCoords[0][0]);
        player = -1;
        drawStone(gridCoords[1][0]);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
