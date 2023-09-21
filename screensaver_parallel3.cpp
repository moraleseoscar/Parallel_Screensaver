#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h> 

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

    double start_time, end_time;
    start_time = omp_get_wtime();

    int numCircles;


    try {
        numCircles = std::stoi(argv[1]); // Obtener el número de círculos desde el argumento
        if(numCircles >= 500) numCircles = numCircles / 2;
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: El argumento no es un número válido." << std::endl;
        return 1;
    }


    if(numCircles <=0 ){
        std::cout << "No se pueden ingresar numeros menores o iguales que 0" << std::endl;
        return 1;
    }

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

    // Arreglos para almacenar los colores de los círculos
    Uint8 circleRed[numCircles];
    Uint8 circleGreen[numCircles];
    Uint8 circleBlue[numCircles];

    // Generar círculos con propiedades aleatorias
    for (int i = 0; i < numCircles; ++i) {
        circleX[i] = rand() % (800 - 2 * circleRadius) + circleRadius;
        circleY[i] = rand() % (600 - 2 * circleRadius) + circleRadius;
        circleSpeedX[i] = rand() % 5 + 1;
        circleSpeedY[i] = rand() % 5 + 1;

        // Generar y almacenar colores aleatorios
        circleRed[i] = rand() % 256;
        circleGreen[i] = rand() % 256;
        circleBlue[i] = rand() % 256;
    }

    Uint32 current_time, last_time = 0, fps = 0;
    Uint32 total_fps = 0;
    Uint32 total_time = 0;

    while (!quit) {
        current_time = SDL_GetTicks();
        if (current_time > last_time + 1000) {
            printf("%d fps\n", fps);
            total_fps += fps;
            total_time += 1000; // Sumar un segundo al tiempo total
            fps = 0;
            last_time = current_time;
        }

        fps++;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpiar el renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        #pragma omp parallel for num_threads(14)
        for (int i = 0; i < numCircles; ++i) {
            circleX[i] += circleSpeedX[i];
            circleY[i] += circleSpeedY[i];

            if (circleX[i] <= circleRadius || circleX[i] >= 800 - circleRadius) {
                circleSpeedX[i] = -circleSpeedX[i];
            }

            if (circleY[i] <= circleRadius || circleY[i] >= 600 - circleRadius) {
                circleSpeedY[i] = -circleSpeedY[i];
            }

            #pragma omp critical
            {
            // Utilizar los colores almacenados para cada círculo
            SDL_SetRenderDrawColor(renderer, circleRed[i], circleGreen[i], circleBlue[i], 255);
            SDL_RenderFillCircle(renderer, circleX[i], circleY[i], circleRadius);
             }
        }

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
    }

    // Calcular y mostrar el promedio de FPS
    if (total_time > 0) {
        float average_fps = static_cast<float>(total_fps) / (total_time / 1000.0f);
        printf("Promedio de FPS: %.2f\n", average_fps);
    }

    // Liberar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    end_time = omp_get_wtime();
    double wave_creation_time = end_time - start_time;
    std::cout << "Tiempo de creación de onda: " << wave_creation_time << " segundos" << std::endl;

    return 0;
}
