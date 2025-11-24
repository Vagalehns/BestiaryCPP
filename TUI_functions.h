//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_TUI_FUNCTIONS_H
#define BESTIARYCPP_TUI_FUNCTIONS_H

#define MAX_MENU_ITEMS 10

#include <format>
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
std::string padString(std::string string_to_pad, unsigned short new_size, char pad_char, bool center);


std::string getStringFromUser(const std::string &question, bool with_confirmation, std::string unallowed_chars);
std::string getStringFromUser(const std::string &question, bool with_confirmation);

void resetListItem();
void printListItem(std::string item, bool reset);
void printListItem(std::string item);

template<std::size_t N>
int getOptionFromUser(const std::array<const char*, N>& options, std::string question) {

    int result;
    bool valid=false;

    do {
        clearConsole();
        std::cout << question << "\n";

        resetListItem();
        for (const auto& opt : options) {
            printListItem(opt);
        }

        std::cout << "\n";
        result = getIntFromUser(1, N, ">", true, true) - 1;

        valid = getConfirmationFromUser("You picked \"" + std::string(options[result]) + "\". Is that correct?");

    } while (!valid);

    return result;
}




#endif //BESTIARYCPP_TUI_FUNCTIONS_H