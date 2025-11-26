//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_BESTIARYAPP_H
#define BESTIARYCPP_BESTIARYAPP_H

#include "TUI_functions.h"
#include "data_storing/databases/Enclosure.h"
#include "data_storing/databases/Keeper.h"
#include "data_storing/databases/Region.h"
#include "data_storing/databases/Species.h"
#include "data_storing/databases/Animal.h"

#include "TUI_functions/Menu.h"

inline bool echo(const std::string &str );

struct AppState{
    bool nofuzz=false;
    bool vocal=false;
    bool help=false;
};

class BestiaryApp {

    public:

        Menu menu_start,
             menu_add_data,
             menu_view_data,
             menu_load_data,
             menu_save_data;

        AppState State;

        RegionDB regionDB;
        SpeciesDB speciesDB;
        KeeperDB keeperDB;
        EnclosureDB enclosureDB;
        AnimalDB animalDB;

        explicit BestiaryApp(AppState state);

        bool Run();

        static void Splashscreen();

        bool saveEverything(std::filesystem::path folder);
        bool loadEverything(std::filesystem::path folder, bool clear_data);
};

#endif //BESTIARYCPP_BESTIARYAPP_H