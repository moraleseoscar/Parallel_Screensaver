#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main(int argc, char* argv[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    // Inicializar SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Log("Error al inicializar SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Crear una ventana
    SDL_Window* window = SDL_CreateWindow(
        "Protector de pantalla",
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

    // Color de fondo: blanco
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Color del texto: negro
    SDL_Color textColor = {0, 0, 0, 255};

    // Cargar la fuente
    TTF_Font* font = TTF_OpenFont("./fonts/minecraft_font.ttf", 36); // Cambia la ruta y el tamaño de la fuente
    if (!font) {
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Crear la superficie del texto
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hola mundo", textColor);
    if (!textSurface) {
        SDL_Log("Error al crear la superficie de texto: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Crear la textura del texto
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("Error al crear la textura de texto: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Limpiar la superficie de texto
    SDL_FreeSurface(textSurface);

    // Obtener el tamaño de la ventana
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Posición del texto en el centro de la ventana
    SDL_Rect textRect = {
        (windowWidth - textSurface->w) / 2,
        (windowHeight - textSurface->h) / 2,
        textSurface->w,
        textSurface->h
    };

    // Bucle principal
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpiar el renderer
        SDL_RenderClear(renderer);

        // Dibujar el texto
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
    }

    // Liberar recursos
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
