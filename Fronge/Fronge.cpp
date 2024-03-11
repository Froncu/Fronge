#include "Console.h"

#include <SDL2/SDL_main.h>

int main(int, char**)
{
    fro::Console::log("Hello world!", fro::Console::TextColor::Black, fro::Console::BackgroundColor::Yellow);
    
    return 0;
}