#include <iostream>
#include <cstdlib>
#include <cstring>

#include "TUI_functions.h"
#include "BestiaryApp.h"


int main(int argc, char **argv) {

    AppState appState;

    //parsing arguments
    for(int i=1; i<argc; ++i){

        if       ( !std::strcmp(argv[i], "--nofuzz")) {
            appState.nofuzz=true;
        }else if ( !std::strcmp(argv[i], "-v") ||
                   !std::strcmp(argv[i], "--vocal")) {
            appState.vocal=true;
        }else if ( !std::strcmp(argv[i], "-h") ||
                   !std::strcmp(argv[i], "--help")) {
            appState.help=true;
        };

        if (appState.vocal) {
            printf("Argument %d : %s\n", i, argv[i]);
        }

    }

    BestiaryApp app(appState);
    app.Run();

    return 0;
}