#include "system/winfunc.h"

#include <iostream>

#if (defined(__WIN32) || defined(__WIN64))
    #define NOMINMAX
    #include <windows.h>
#endif

namespace civilarium::system::winfunc {

namespace {

#if (defined(__WIN32) || defined(__WIN64))
    using FuncType1 = BOOL (WINAPI *)();
    using FuncType2 = HRESULT (WINAPI *)(int);

    enum class ProcessDpiAwareness : unsigned char {
        kProcessDpiUnaware = 0,
        kProcessSystemDpiAware = 1,
        kProcessPerMonitorDpiAware = 2,
    };

    FuncType1 GetSetProcessDPIAware()
    {
        FuncType1 set_process_dpi_aware = nullptr;
        HMODULE user_dll = LoadLibraryA("User32.dll");
        if (user_dll != nullptr) {
            set_process_dpi_aware =
                (FuncType1)GetProcAddress(user_dll, "SetProcessDPIAware");
        }
        return set_process_dpi_aware;
    }

    FuncType2 GetSetProcessDpiAwareness()
    {
        FuncType2 set_process_dpi_awareness = nullptr;
        HMODULE shcore_dll = LoadLibraryA("Shcore.dll");
        if (shcore_dll != nullptr) {
            set_process_dpi_awareness =
                (FuncType2)GetProcAddress(
                    shcore_dll, "SetProcessDpiAwareness");
        }
        return set_process_dpi_awareness;
    }
#endif

}  // namespace

/*  References:
 *      Eric Wasylishen.
 *      https://discourse.libsdl.org/t/sdl-getdesktopdisplaymode-resolution-
 *      reported-in-windows-10-when-using-app-scaling/22389/4,
 *      (accessed 2024-11-02).
 */
void ResolveBlurriness()
{
    #if (defined(__WIN32) || defined(__WIN64))
        std::cout << "[ResolveBlurriness()]" << std::endl;
        std::cout << "  Start to resolve the blurriness of the screen."
                  << std::endl;
        bool succeeds = true;

        // Get required processes from DLLs.
        FuncType1 set_process_dpi_aware = GetSetProcessDPIAware();
        FuncType2 set_process_dpi_awareness = GetSetProcessDpiAwareness();

        // Resolve the blurriness of the screen
        if (set_process_dpi_awareness != nullptr) {
            std::cout << "  Access SetProcessDpiAwareness." << std::endl;
            HRESULT result = set_process_dpi_awareness(
                static_cast<int>(
                    ProcessDpiAwareness::kProcessPerMonitorDpiAware));
            if (result != S_OK) {
                succeeds = false;
                std::cout << "  Failed in accessing SetProcessDpiAwareness..."
                          << std::endl;
            }
        } else if (set_process_dpi_aware != nullptr) {
            std::cout << "  Access SetProcessDPIAware." << std::endl;
            BOOL result = set_process_dpi_aware();
            if (result == 0) {
                succeeds = false;
                std::cout << "  Failed in accessing SetProcessDPIAware..."
                          << std::endl;
            }
        }

        // Report the result
        std::cout << "  " << (succeeds ? "Succeeded" : "Failed") << " "
                  << "in resolving the blurriness of the screen."
                  << std::endl;
    #endif
}

}  // namespace civilarium::system::winfunc
