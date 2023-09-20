#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <omp.h>

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Uso: " << argv[0] << " <número_de_círculos>" << std::endl;
        return 1;
    }

    int numCircles = std::stoi(argv[1]);

    if (numCircles <= 0) {
        std::cout << "No se pueden ingresar números menores o iguales que 0" << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Círculos Rebote",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Error al crear la ventana: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Error al crear el renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<int> circleX(numCircles);
    std::vector<int> circleY(numCircles);
    int circleRadius = 30;

    std::vector<int> circleSpeedX(numCircles);
    std::vector<int> circleSpeedY(numCircles);

    #pragma omp parallel for
    for (int i = 0; i < numCircles; ++i) {
        circleX[i] = rand() % (800 - 2 * circleRadius) + circleRadius;
        circleY[i] = rand() % (600 - 2 * circleRadius) + circleRadius;
        circleSpeedX[i] = rand() % 5 + 1;
        circleSpeedY[i] = rand() % 5 + 1;
    }

    Uint32 current_time, last_time = 0, fps = 0;
    Uint32 total_fps = 0;
    Uint32 total_time = 0;

    while (!quit) {
        current_time = SDL_GetTicks();
        if (current_time > last_time + 1000) {
            printf("%d fps\n", fps);
            total_fps += fps;
            total_time += 1000;
            fps = 0;
            last_time = current_time;
        }

        fps++;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        #pragma omp parallel for
        for (int i = 0; i < numCircles; ++i) {
            circleX[i] += circleSpeedX[i];
            circleY[i] += circleSpeedY[i];

            if (circleX[i] <= circleRadius || circleX[i] >= 800 - circleRadius) {
                circleSpeedX[i] = -circleSpeedX[i];
            }

            if (circleY[i] <= circleRadius || circleY[i] >= 600 - circleRadius) {
                circleSpeedY[i] = -circleSpeedY[i];
            }

            Uint8 red = rand() % 256;
            Uint8 green = rand() % 256;
            Uint8 blue = rand() % 256;

            #pragma omp critical
            {
                SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
                SDL_RenderFillCircle(renderer, circleX[i], circleY[i], circleRadius);
            }
        }

        SDL_RenderPresent(renderer);
    }

    if (total_time > 0) {
        float average_fps = static_cast<float>(total_fps) / (total_time / 1000.0f);
        printf("Promedio de FPS: %.2f\n", average_fps);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
