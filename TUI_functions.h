//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_TUI_FUNCTIONS_H
#define BESTIARYCPP_TUI_FUNCTIONS_H

#define MAX_MENU_ITEMS 10

#include <functional>
#include <iostream>
#include <string>
#include <vector>

void clearConsole();

std::string generateTable(std::vector<std::string> data, int columns, bool borders=false);

int getIntFromUser(int min, int max, const std::string &question, bool has_upper_bound, bool has_lower_bound);
bool getConfirmationFromUser(const std::string &question);

bool checkForChar(const std::string &string, char c);
bool validateString(const std::string &string, const std::string &unallowed_chars);

std::string getStringFromUser(const std::string &question, bool with_confirmation, std::string unallowed_chars);
std::string getStringFromUser(const std::string &question, bool with_confirmation);


typedef struct  {
    std::string title;
    std::function<bool()> screen_to_call;
} MenuItem;

class Menu {
    public:
        MenuItem menu_items[MAX_MENU_ITEMS];
        char item_count=0;
        std::string menu_title;
        bool should_close=false;

        explicit Menu(std::string title, std::string exit_name="Back");


        bool addItem(MenuItem item);

        void open();
        void open(bool clear);

        bool close();

    private:
        int translateFromUserToIndex(int user_sel) const {
            return item_count-user_sel;
        }
};

#endif //BESTIARYCPP_TUI_FUNCTIONS_H