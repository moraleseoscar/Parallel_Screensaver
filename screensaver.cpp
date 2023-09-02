#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

// Funcion que genera el circulo
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
    // Comprobar si se proporcionó un argumento
    if (argc != 2) {
        std::cout << "Uso: " << argv[0] << " <número_de_círculos>" << std::endl;
        return 1;
    }

    int numCircles = std::stoi(argv[1]); // Obtener el número de círculos desde el argumento

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    // Crear una ventana
    SDL_Window* window = SDL_CreateWindow(
        "Círculos Rebote",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, // Tamaño de la ventana
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Error al crear la ventana: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Crear un renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Error al crear el renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Bucle principal
    bool quit = false;
    SDL_Event event;

    // Semilla para generar colores aleatorios
    srand(static_cast<unsigned int>(time(nullptr)));

    // Arreglos para almacenar las propiedades de los círculos
    int circleX[numCircles];
    int circleY[numCircles];
    int circleRadius = 30;

    // Velocidades de movimiento aleatorias
    int circleSpeedX[numCircles];
    int circleSpeedY[numCircles];

    // Generar círculos con propiedades aleatorias
    for (int i = 0; i < numCircles; ++i) {
        circleX[i] = rand() % (800 - 2 * circleRadius) + circleRadius;
        circleY[i] = rand() % (600 - 2 * circleRadius) + circleRadius;
        circleSpeedX[i] = rand() % 5 + 1;
        circleSpeedY[i] = rand() % 5 + 1;
    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpiar el renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

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
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);

            SDL_RenderFillCircle(renderer, circleX[i], circleY[i], circleRadius);
        }

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
    }

    // Liberar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
