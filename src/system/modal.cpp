#include "system/modal.h"

#include <iostream>
#include <string>

#include "SDL2/SDL.h"

namespace civilarium::system::modal {

void ShowErrorMessage(const char title[], const char message[])
{
    std::cerr << "[" << title << "] " << message << std::endl;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

void ShowErrorMessage(
    const char title[], const char message[], const char quotation[])
{
    std::string full_message = message;
    full_message.append("\n> ");
    full_message.append(quotation);
    ShowErrorMessage(title, full_message.c_str());
}

}  // namespace civilarium::system::modal
