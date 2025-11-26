//
// Created by armands on 14.11.25.
//

#include "BestiaryApp.h"
#include "info.h"
#include "styling_functions.h"

inline bool echo(const std::string &str ){
    std::cout << str << std::endl;
    return true;
};

template<class DB>
auto makeAdd(DB& db, std::string func_name) {
    return [&, func_name]() -> MenuReturn {
        if (db.addByForm())
            return {STAY_SHOW_MSG, func_name + " added"};
        else
            return {STAY_SHOW_ERROR, "Failed to add " + func_name};
    };
}

template<class DB>
auto makeView(DB& db) {
    return [&]() -> MenuReturn {
        db.baseView();
        return {STAY, ""};
    };
}



BestiaryApp::BestiaryApp(AppState state) :

    speciesDB(
        std::bind_front(&RegionDB::getByID, &regionDB),
        std::bind_front(&RegionDB::pickByUser, &regionDB)
    ),

    animalDB(
        std::bind_front(&SpeciesDB::getByID, &speciesDB),
        std::bind_front(&SpeciesDB::pickByUser, &speciesDB),

        std::bind_front(&KeeperDB::getByID, &keeperDB),
        std::bind_front(&KeeperDB::pickByUser, &keeperDB),

        std::bind_front(&EnclosureDB::getByID, &enclosureDB),
        std::bind_front(&EnclosureDB::pickByUser, &enclosureDB)
    ),

    menu_start("Main menu", "Exit"),
    menu_add_data("Add data", "Back"),
    menu_view_data("View data", "Back"),
    menu_load_data("Load data", "Back"),
    menu_save_data("Save data", "Back"){


    State = state;

    menu_save_data.addItem({"Save EVERYTGING!!!", [this]()->MenuReturn {
            const std::string loc = "C:\\Users\\arman\\Desktop\\Test";
            this->animalDB.saveToCSVFile("animaldb", loc);
            // this->enclosureDB.saveToCSVFile("enclosuredb", loc);
            this->keeperDB.saveToCSVFile("keeperdb", loc);
            // this->regionDB.saveToCSVFile("regiondb", loc);
            // this->speciesDB.saveToCSVFile("speciesdb", loc);
            return {STAY_SHOW_MSG, ":)"};
    }});

    menu_load_data.addItem({"Load EVERYTGING!!!", [this]()->MenuReturn {
            const std::string loc = "C:\\Users\\arman\\Desktop\\Test";
            this->animalDB.readFromCSVFile("animaldb", loc);
            this->enclosureDB.readFromCSVFile("enclosuredb", loc);
            this->keeperDB.readFromCSVFile("keeperdb", loc);
            this->speciesDB.readFromCSVFile("speciesdb", loc);
            this->regionDB.readFromCSVFile("regiondb", loc);
            return {STAY_SHOW_MSG, ":)"};
    }});

    menu_add_data.addItem({"Add region",    makeAdd(regionDB, "Region")});
    menu_add_data.addItem({"Add species",   makeAdd(speciesDB, "Species")});
    menu_add_data.addItem({"Add animal",    makeAdd(animalDB, "Animal")});
    menu_add_data.addItem({"Add keeper",    makeAdd(keeperDB, "Keeper")});
    menu_add_data.addItem({"Add enclosure", makeAdd(enclosureDB, "Enclosure")});

    menu_view_data.addItem({"View region",    makeView(regionDB)});
    menu_view_data.addItem({"View species",   makeView(speciesDB)});
    menu_view_data.addItem({"View animal",    makeView(animalDB)});
    menu_view_data.addItem({"View keeper",    makeView(keeperDB)});
    menu_view_data.addItem({"View enclosure", makeView(enclosureDB)});

    menu_view_data.addItem({"View feeding", []()->MenuReturn{return MenuReturn(STAY);}});

    menu_start.addItem({"Add data", [this]()->MenuReturn{return this->menu_add_data.open();}});
    menu_start.addItem({"View data", [this]()->MenuReturn{return this->menu_view_data.open();}});
    menu_start.addItem({"Load data", [this]()->MenuReturn{return this->menu_load_data.open();}});
    menu_start.addItem({"Save data", [this]()->MenuReturn{return this->menu_save_data.open();}});
}

bool BestiaryApp::Run() {

    if (State.help) {
        std::cout << HELP_MESSEGE;
        return true;
    }

    if (!State.nofuzz) {
        Splashscreen();
    }

    menu_start.open();

    std::cout << "App Closed";

    return true;
}

void BestiaryApp::Splashscreen() {

        std::cout << "\n" << BRIGHTGREENSTY << BOLDSTY << SPLASHSCREEN << ENDSTY << "\n";
        std::cout << "                " << APP_USECASE << "\n\n";
        std::cout << "Version: " << APP_VERSION << "\n";
        std::cout << "Made by: " << APP_AUTHOR << "\n";
        std::cout << "Copyright " << APP_UPDATE_YEAR << "\n\n";
        std::cout << "===================================";
        std::cout << "\nPress Enter to continue or e to exit...";


        int user_input;

        do {
            user_input = std::cin.get();
        } while (user_input != '\n' && user_input != 'e');
        if (user_input == 'e') {
            std::exit(0);
        }

};