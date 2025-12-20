#include <cstring>
#include <windows.h>

#include "App.h"

int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8);

    AppState appState;

    //parsing arguments
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--nofuzz")) {
            appState.nofuzz = true;
        } else if (!std::strcmp(argv[i], "-v") ||
                   !std::strcmp(argv[i], "--vocal")) {
            appState.vocal = true;
        } else if (!std::strcmp(argv[i], "-h") ||
                   !std::strcmp(argv[i], "--help")) {
            appState.help = true;
        };

        if (appState.vocal) {
            printf("Argument %d : %s\n", i, argv[i]);
        }
    }


    App app(appState);
    app.Run();

    return 0;
}
