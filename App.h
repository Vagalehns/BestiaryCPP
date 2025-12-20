//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_BESTIARYAPP_H
#define BESTIARYCPP_BESTIARYAPP_H

#include <filesystem>

#include "data_storing/databases/Enclosure.h"
#include "data_storing/databases/Keeper.h"
#include "data_storing/databases/Region.h"
#include "data_storing/databases/Species.h"
#include "data_storing/databases/Animal.h"

#include "TUI_functions/Menu.h"


struct AppState {
    bool nofuzz = false;
    bool vocal = false;
    bool help = false;
    bool autosave = false;
};

class App {
private:
    const fs::path def_path;

public:
    Menu menu_start,
            menu_add_data,
            menu_view_data,
            menu_load_data,
            menu_save_data,
            menu_edit_data,
            menu_delete_data,
            menu_calculations,
            deletion_submenu;

    AppState State;

    RegionDB regionDB;
    SpeciesDB speciesDB;
    KeeperDB keeperDB;
    EnclosureDB enclosureDB;
    AnimalDB animalDB;

    explicit App(AppState state);

    bool Run();

    static void Splashscreen();

    bool saveEverything(const std::filesystem::path &folder);

    bool loadEverything(const std::filesystem::path &folder, bool clear_data);
};

#endif //BESTIARYCPP_BESTIARYAPP_H
