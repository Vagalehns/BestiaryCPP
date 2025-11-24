//
// Created by arman on 11/24/2025.
//

#include "Menu.h"
#include "../TUI_functions.h"
#include "../styling_functions.h"

Menu::Menu(std::string title, std::string exit_name)  {

    this->menu_title = std::move(title);

    this->addItem({BRIGHTREDSTY+std::move(exit_name), []()->MenuReturn {
        return MenuReturn(BACK);
    }});

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

void Menu::renderMenu() {
    render.str("");
    render<<BOLDSTY BGBRIGHTGREENSTY BLACKSTY << "~{ " << menu_title << " }~" << ENDSTY << "\n";

    for (int i = item_count - 1; i >= 0; i--) {
        render << (item_count - i) << ". " << BRIGHTGREENSTY << menu_items[i].title << ENDSTY << "\n";
    }
}


MenuReturn Menu::open(){

    // Items are displayed in the reverse order
    renderMenu();

    clearConsole();
    std::cout << render.str();

    MenuReturn menu_return(STAY);

    should_close=false;
    while (!should_close) {
        int selection = getIntFromUser(1, item_count, ">", true, true);

        menu_return = menu_items[this->translateFromUserToIndex(selection)].screen_to_call();


        switch (menu_return.state) {
            case STAY:
                clearConsole();
                std::cout << render.str();
                break;
            case STAY_SHOW_MSG:
                clearConsole();
                std::cout << render.str() << BGBLUESTY << "\n" << menu_return.message << ENDSTY << "\n";
                break;
            case STAY_SHOW_ERROR:
                clearConsole();
                std::cout << render.str() << BGREDSTY << "\n!!!" << menu_return.message << "!!!" << ENDSTY << "\n";
                break;
            case BACK:
                should_close=true;
                break;
        }

    }

    clearConsole();

    return MenuReturn(STAY);

}