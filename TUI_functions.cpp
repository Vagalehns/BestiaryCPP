//
// Created by armands on 14.11.25.
//

#include "TUI_functions.h"
#include <cstdlib>
#include <iostream>
#include <utility>

#include "styling_functions.h"

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int getIntFromUser(int min, int max, std::string question, bool has_upper_bound, bool has_lower_bound) {

    int input;
    bool valid_input = false;


    while (!valid_input) {
        std::cout << question;
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();

        } else {
            if (has_lower_bound && input < min) {
                valid_input = false;
            } else if (has_upper_bound && input > max) {
                valid_input = false;
            } else {
                valid_input = true;
            }
        }

        std::cin.ignore(10000, '\n');
    }

    return input;
};

#include <string>

Menu::Menu(std::string title, std::string exit_name)  {
    this->menu_title = std::move(title);

    this->addItem({BRIGHTREDSTY+std::move(exit_name), [this]() { return this->close(); }});
};

bool Menu::addItem(MenuItem item) {

    if (item_count >= MAX_MENU_ITEMS) {
        return false;
    };

    menu_items[item_count] = std::move(item);
    item_count++;

    return true;
}

bool Menu::close() {

    this->should_close = true;
    return true;

}

void Menu::open() {
    this->open(true);
}

void Menu::open(bool clear){

    if (clear) {
        clearConsole();
    }

    std::cout << BOLDSTY BGBRIGHTGREENSTY BLACKSTY << "~{ " << menu_title << " }~" << ENDSTY << "\n";

    // Items are displayed in the reverse order
    for (int i = item_count - 1; i >= 0; i--) {
        std::cout << (item_count - i) << ". " << BRIGHTGREENSTY << menu_items[i].title << ENDSTY << "\n";
    }


    int selection;
    bool fine;

    while (!should_close) {
        selection=getIntFromUser(1, item_count, ">", true, true);
        fine=menu_items[ this->translateFromUserToIndex(selection) ].screen_to_call();

        if (!fine) {
            std::cout << "\nCouldn't fulfill the action\n";
        };
    }

    if (clear) {
        clearConsole();
    }

}
