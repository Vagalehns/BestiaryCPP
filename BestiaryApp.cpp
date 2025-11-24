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


BestiaryApp::BestiaryApp(AppState state) :

    speciesDB( [this](KeyID id) -> Region* {
        return regionDB.getByID(id);
    },
    [this]() -> KeyID {
        return regionDB.pickByUser();
    }),

    menu_start("Main menu", "Exit"),
    menu_add_data("Add data", "Back"),
    menu_view_data("View data", "Back"){


    State = state;

    menu_add_data.addItem({"Add region", [this]()->MenuReturn {

            if (regionDB.addByForm()) {
                return MenuReturn(STAY_SHOW_MSG, "Region added");
            }else {
                return MenuReturn(STAY_SHOW_ERROR, "Failed to add region");
            }

    }});

    menu_add_data.addItem({"Add species", [this]()->MenuReturn {

            if (speciesDB.addByForm()) {
                return MenuReturn(STAY_SHOW_MSG, "Species added");
            }else {
                return MenuReturn(STAY_SHOW_ERROR, "Failed to add species");
            }

    }});

    menu_add_data.addItem({"Add animal", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_add_data.addItem({"Add keeper", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_add_data.addItem({"Add enclosure", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_add_data.addItem({"Add feeding", []()->MenuReturn{return MenuReturn(STAY);}});

    menu_view_data.addItem({"View region", [this]()->MenuReturn {
        regionDB.baseView();
        return MenuReturn(STAY);
    }});

    menu_view_data.addItem({"View species", [this]()->MenuReturn {
        speciesDB.baseView();
        return MenuReturn(STAY);
    }});

    menu_view_data.addItem({"View animal", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_view_data.addItem({"View keeper", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_view_data.addItem({"View enclosure", []()->MenuReturn{return MenuReturn(STAY);}});
    menu_view_data.addItem({"View feeding", []()->MenuReturn{return MenuReturn(STAY);}});

    menu_start.addItem({"Add data", [this]()->MenuReturn{return this->menu_add_data.open();}});
    menu_start.addItem({"View data", [this]()->MenuReturn{return this->menu_view_data.open();}});
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