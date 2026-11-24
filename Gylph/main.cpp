#include <fstream>
#include <iostream>
#include <memory_resource>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include <SDL3/SDL.h>

#include "tokenization.h"
#include "parser.h"
#include "debug.h"
#include "css.h"
#include "style.h"
#include "layout.h"
#include "display.h"

namespace {

    std::string ReadEntireFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open input file: " << path << "\n";
            std::exit(EXIT_FAILURE);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void DrawDisplayList(SDL_Renderer* renderer, const DisplayList& list) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (const auto& entry : list) {
            std::visit([renderer](const auto& cmd) {
                using T = std::decay_t<decltype(cmd)>;
                if constexpr (std::is_same_v<T, SolidColor>) {
                    // Adjust if your Color channels are floats (0.0f..1.0f) via SDL_SetRenderDrawColorFloat
                    SDL_SetRenderDrawColor(
                        renderer,
                        static_cast<Uint8>(cmd.color.r),
                        static_cast<Uint8>(cmd.color.g),
                        static_cast<Uint8>(cmd.color.b),
                        static_cast<Uint8>(cmd.color.a)
                    );

                    SDL_FRect sdlRect{
                        static_cast<float>(cmd.rect.x),
                        static_cast<float>(cmd.rect.y),
                        static_cast<float>(cmd.rect.width),
                        static_cast<float>(cmd.rect.height)
                    };

                    SDL_RenderFillRect(renderer, &sdlRect);
                }
                }, entry.color);
        }
    }

} // namespace

int main(int argc, char* argv[]) {
    std::string htmlPath = (argc > 1) ? argv[1] : "test.html";
    std::string cssPath = (argc > 2) ? argv[2] : "css.css";

    std::string htmlSource = ReadEntireFile(htmlPath);
    std::string cssSource = ReadEntireFile(cssPath);

    // Keep arena alive for the full lifetime of parsed DOM nodes and StyledNode pointers
    std::pmr::monotonic_buffer_resource arena(4 * 1024 * 1024); // 4 MB
    std::pmr::polymorphic_allocator<char> genericAlloc(&arena);

    Tokenizator tokenizer(htmlSource);
    auto tokens = tokenizer.Tokenize();

    HTMLParser htmlParser(tokens, genericAlloc);
    auto nodes = htmlParser.ParseNodes();
    if (nodes.empty()) {
        std::cerr << "Error: Parsed HTML produced no nodes.\n";
        return EXIT_FAILURE;
    }

    CSSParser cssParser(cssSource);
    auto styleSheet = cssParser.GetStyleSheet();

    Style styleBuilder;
    auto styledTree = styleBuilder.BuildTree(nodes[0], styleSheet);

    auto layoutTree = LayoutBox::BuildLayoutTree(&styledTree);
    if (!layoutTree) {
        std::cerr << "Error: Root layout box is empty or display:none.\n";
        return EXIT_FAILURE;
    }

    // Initial viewport / containing block dimensions
    constexpr int kWindowWidth = 1024;
    constexpr int kWindowHeight = 768;

    Dimension viewport{};
    viewport.content.x = 0.0;
    viewport.content.y = 0.0;
    viewport.content.width = static_cast<double>(kWindowWidth);
    viewport.content.height = 0.f;

    if (layoutTree) {
        layoutTree->Layout(viewport);
    }

    DisplayList displayList = BuildDisplayList(*layoutTree);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Glyph - Layout & Display List",
        kWindowWidth,
        kWindowHeight,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bool running = true;
    SDL_Event event{};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                viewport.content.width = static_cast<double>(event.window.data1);
                viewport.content.height = static_cast<double>(event.window.data2);
                // Re-run layout & rebuild display list on resize
                layoutTree->Layout(viewport);
                displayList = BuildDisplayList(*layoutTree);
            }
        }

        // Clear window to white before drawing display list
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        DrawDisplayList(renderer, displayList);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}