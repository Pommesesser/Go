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
SDL_Texture *stoneTextures[2];

void preComputeGridCoordinates() {
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y)
            gridCoords[x][y] = (SDL_Point) {border + x * offset, border + y * offset};
    }
}

void preComputeStoneTextures() {
    // Black stone
    stoneTextures[0] = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, stoneRadius*2, stoneRadius*2);
    SDL_SetTextureBlendMode(stoneTextures[0], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, stoneTextures[0]);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    for (int x = -stoneRadius; x <= stoneRadius; x++)
        for (int y = -stoneRadius; y <= stoneRadius; y++)
            if (x*x + y*y <= stoneRadius*stoneRadius)
                SDL_RenderDrawPoint(ren, x + stoneRadius, y + stoneRadius);

    // White stone
    stoneTextures[1] = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, stoneRadius*2, stoneRadius*2);
    SDL_SetTextureBlendMode(stoneTextures[1], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, stoneTextures[1]);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int x = -stoneRadius; x <= stoneRadius; x++)
        for (int y = -stoneRadius; y <= stoneRadius; y++)
            if (x*x + y*y <= stoneRadius*stoneRadius)
                SDL_RenderDrawPoint(ren, x + stoneRadius, y + stoneRadius);

    SDL_SetRenderTarget(ren, nullptr);
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

void drawStones() {
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            if (grid[i][j] == 0)
                continue;

            SDL_Rect dst = {
                gridCoords[i][j].x - stoneRadius,
                gridCoords[i][j].y - stoneRadius,
                stoneRadius * 2,
                stoneRadius * 2
            };

            SDL_RenderCopy(ren, stoneTextures[grid[i][j] - 1], nullptr, &dst);
        }
    }
}

void handleClick() {

}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Go",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dim, dim, SDL_WINDOW_RESIZABLE);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    preComputeGridCoordinates();
    preComputeStoneTextures();

    grid[4][4] = 1;

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                quit = 1;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT)
                    printf("x:%d y:%d\n", event.button.x, event.button.y);
            }
        }

        drawBackground();
        drawGrid();
        drawStones();
        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
