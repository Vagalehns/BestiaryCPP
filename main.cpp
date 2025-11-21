#include <iostream>
#include <cstring>
#include <fstream>

#include "BestiaryApp.h"
#include "Animal.h"

int main(int argc, char **argv) {

    SpeciesDB speciesDB;
    auto speciesResolver = [&speciesDB](KeyID ID) -> Species* {
        return speciesDB.getByID(ID);
    };
    AnimalDB animalDB(speciesResolver);
    AnimalDB animal_db2(speciesResolver);

    speciesDB.appendAutoID({"Tiger", "Panthera tigris", 'M'});
    speciesDB.appendAutoID({"Dog", "Canis lupus familiaris", 'M'});
    speciesDB.appendAutoID({"Cat", "Felis catus", 'M'});
    speciesDB.appendAutoID({"Eagle", "Aquila chrysaetos", 'B'});
    speciesDB.appendAutoID({"Dolphin", "Tursiops truncatus", 'M'});
    speciesDB.appendAutoID({"Python", "Python regius", 'R'});
    speciesDB.appendAutoID({"Parrot", "Psittacus erithacus", 'B'});
    speciesDB.appendAutoID({"Elephant", "Loxodonta africana", 'M'});
    speciesDB.appendAutoID({"Penguin", "Aptenodytes forsteri", 'B'});
    speciesDB.appendAutoID({"Crocodile", "Crocodylus niloticus", 'R'});

    auto stringAssertion = [](std::string A, std::string B) -> int {
        return B.compare(A);

    };

    speciesDB.order(&Species::species, stringAssertion);

    speciesDB.display();


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
