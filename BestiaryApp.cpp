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


BestiaryApp::BestiaryApp(AppState state) : StartMenu("Main Menu", "Exit") {

    State = state;

    StartMenu.addItem({"Add animal species", [](){return echo("Item 1 called");} });
    StartMenu.addItem({"Item 2", [](){ return echo("Item 2 called"); }});
    StartMenu.addItem({"Item 3", [](){ return echo("Item 3 called"); }});
    StartMenu.addItem({"Item 4", [](){ return echo("Item 4 called"); }});
    StartMenu.addItem({"Item 5", [](){ return echo("Item 5 called"); }});
    StartMenu.addItem({"Item 6", [](){ return echo("Item 6 called"); }});

}

bool BestiaryApp::Run() {

    if (State.help) {
        std::cout << HELP_MESSEGE;
        return true;
    }

    if (!State.nofuzz) {
        Splashscreen();
    }

    StartMenu.open();
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