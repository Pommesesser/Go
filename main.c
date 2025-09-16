#include <stdio.h>
#include <SDL.h>

constexpr int width = 900;
constexpr int height = 900;
constexpr int border = 50;
constexpr int gridSize = 9;
int grid[gridSize][gridSize] = {0};

int offset;
int stoneRad;
int gridDim;

SDL_Window *win;
SDL_Renderer *ren;

SDL_Texture *stoneTexArr[2];
SDL_Texture *gridTex;
SDL_Texture *bkgTex;

SDL_Rect stoneDestArr[gridSize][gridSize];
SDL_Rect gridDest;
SDL_Rect bkgDest;

int squaredEuclidDist(const SDL_Point a, const SDL_Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void preComputeStoneTextures() {
    // Black stone
    stoneTexArr[0] = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, stoneRad * 2, stoneRad * 2);
    SDL_SetTextureBlendMode(stoneTexArr[0], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, stoneTexArr[0]);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    for (int x = -stoneRad; x <= stoneRad; x++)
        for (int y = -stoneRad; y <= stoneRad; y++)
            if (squaredEuclidDist((SDL_Point){x, y}, (SDL_Point){0, 0}) <= stoneRad*stoneRad)
                SDL_RenderDrawPoint(ren, x + stoneRad, y + stoneRad);

    // White stone
    stoneTexArr[1] = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, stoneRad * 2, stoneRad * 2);
    SDL_SetTextureBlendMode(stoneTexArr[1], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, stoneTexArr[1]);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int x = -stoneRad; x <= stoneRad; x++)
        for (int y = -stoneRad; y <= stoneRad; y++)
            if (squaredEuclidDist((SDL_Point){x, y}, (SDL_Point){0, 0}) <= stoneRad*stoneRad)
                SDL_RenderDrawPoint(ren, x + stoneRad, y + stoneRad);

    SDL_SetRenderTarget(ren, nullptr);
}

void preComputeGridTexture() {
    gridTex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, gridDim, gridDim);
    SDL_SetTextureBlendMode(gridTex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, gridTex);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    for (int i = 0; i < gridSize; ++i) {
        SDL_RenderDrawLine(ren, 0, offset * i, gridDim, offset * i);
        SDL_RenderDrawLine(ren, offset * i, 0, offset * i, gridDim);
    }

    SDL_SetRenderTarget(ren, nullptr);
}

void preComputeBackgroundTexture() {
    bkgTex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(ren, bkgTex);
    SDL_SetRenderDrawColor(ren, 194, 136, 78, 255);
    SDL_RenderClear(ren);

    SDL_SetRenderTarget(ren, nullptr);
}

void drawBackground() {
    SDL_RenderCopy(ren, bkgTex, nullptr, &bkgDest);
    SDL_RenderCopy(ren, gridTex, nullptr, &gridDest);
}

void drawStones() {
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            if (grid[i][j] == 0)
                continue;
            SDL_RenderCopy(ren, stoneTexArr[grid[i][j] - 1], nullptr, &stoneDestArr[i][j]);
        }
    }
}

void handleClick(const SDL_Point c, const int player) {
    const int localX = c.x - gridDest.x;
    const int localY = c.y - gridDest.y;

    const int i = (localX + offset / 2) / offset;
    const int j = (localY + offset / 2) / offset;

    if (i < 0 || i >= gridSize || j < 0 || j >= gridSize)
        return;

    grid[i][j] = (grid[i][j] == 0) ? player : 0;
}

void computeLayout(const int w, const int h) {
    bkgDest.w = w;
    bkgDest.h = h;

    gridDim = (w < h ? w : h) + 1 - border * 2;
    gridDest.x = w / 2 - gridDim / 2;
    gridDest.y = h / 2 - gridDim / 2;
    gridDest.w = gridDim;
    gridDest.h = gridDim;

    offset = gridDim / (gridSize - 1);
    stoneRad = offset / 2 - 5;

    for (int i = 0; i < gridSize; ++i)
        for (int j = 0; j < gridSize; ++j)
            stoneDestArr[i][j] = (SDL_Rect) {gridDest.x + i * offset - stoneRad, gridDest.y + j * offset - stoneRad, stoneRad * 2, stoneRad * 2};
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Go", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_BORDERLESS);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    computeLayout(width, height);
    preComputeStoneTextures();
    preComputeGridTexture();
    preComputeBackgroundTexture();

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                quit = 1;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    printf("LMB: %d %d\n", event.button.x, event.button.y);
                    handleClick((SDL_Point){event.button.x, event.button.y}, 1);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    printf("RMB: %d %d\n", event.button.x, event.button.y);
                    handleClick((SDL_Point){event.button.x, event.button.y}, 2);
                }
            }
        }

        drawBackground();
        drawStones();
        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}