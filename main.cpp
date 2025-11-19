#include <iostream>
#include <cstring>
#include <fstream>

#include "TUI_functions.h"
#include "BestiaryApp.h"
#include "animalFunctions.h"

int main(int argc, char **argv) {


    // Create a vector (list) of Animal objects
    std::vector<Animal> animals = {
        Animal("Buddy", "Canine", 1001, 5),
        Animal("Whiskers", "Feline", 1002, 3),
        Animal("Tweety", "Avian", 1003, 2),
        Animal("Nemo", "Fish", 1004, 1),
        Animal("Thunder", "Equine", 1005, 7)
    };

    std::ofstream outFile("example.csv");

    // Loop through the list and display each animal's details
    for (const auto& animal : animals) {
        outFile<<animal.savableString()<<std::endl;
    }

    outFile.close();

    return 0;


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
