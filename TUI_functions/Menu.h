//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_MENU_H
#define BESTIARYCPP_MENU_H
#include <functional>
#include <sstream>
#include <string>

#define MAX_MENU_ITEMS 10

enum MenuReturnState {
    STAY, STAY_SHOW_MSG, STAY_SHOW_ERROR, BACK
};

struct MenuReturn {
    MenuReturnState state;
    std::string message;

    MenuReturn(MenuReturnState state, const std::string &message) {
        this->state = state;
        this->message = message;
    }

    explicit MenuReturn(MenuReturnState state) {
        this->state = state;
    }
};

typedef struct {
    std::string title;
    std::function<MenuReturn()> screen_to_call;
} MenuItem;

class Menu {
public:
    MenuItem menu_items[MAX_MENU_ITEMS];
    char item_count = 0;
    std::string menu_title;
    bool should_close = false;
    std::ostringstream render;

    explicit Menu(std::string title, std::string exit_name = "Back");

    bool addItem(MenuItem item);

    void renderMenu();


    MenuReturn open();

    bool close();

private:
    int translateFromUserToIndex(int user_sel) const {
        return item_count - user_sel;
    }
};

#endif //BESTIARYCPP_MENU_H
