//
// Created by armands on 14.11.25.
//

#include "TUI_functions.h"
#include <cstdlib>
#include <iostream>
#include <utility>

#include "styling_functions.h"
#include <string>


void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void printListItem(std::string item, bool reset) {
    static int index = 1;

    if (reset) {
        index = 1;
        return;
    }

    std::cout << "\n" << index << ")\t" << item ;
    index++;
};

void printListItem(std::string item) {
    printListItem(item, false);
};

void resetListItem() {
    printListItem("", true);
};


std::string padString(std::string string_to_pad, unsigned short new_size, char pad_char, bool center) {

    unsigned short padding_amount=new_size - string_to_pad.length();

    std::string left_padding;
    std::string right_padding;

    if (center) {
        left_padding = std::string(padding_amount/2, pad_char);
        right_padding = std::string(padding_amount/2 + padding_amount%2, pad_char);
    }else {
        left_padding = std::string(padding_amount, pad_char);
    }

    return left_padding+string_to_pad+right_padding;
};




int getIntFromUser(int min, int max, const std::string &question, bool has_upper_bound, bool has_lower_bound) {

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

bool getConfirmationFromUser(const std::string &question) {
    char input;
    bool valid_input = false;
    bool output=false;


    while (!valid_input) {

        std::cout << question;
        std::cout << "[y/n]";

        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();

        } else {
            if (input=='Y'||input=='y') {
                output = true;
                valid_input = true;
            } else if (input=='N'||input=='n') {
                output = false;
                valid_input = true;
            }
        }

        std::cin.ignore(10000, '\n');
    }

    return output;
};

bool checkForChar(const std::string &string, char c) {
    return string.find(c) != std::string::npos;
};

bool validateString(const std::string &string, const std::string &unallowed_chars) {

    for (int i = 0; i < unallowed_chars.length(); i++) {

        if (checkForChar(string, unallowed_chars[i])) {
            return false;
        }
    }

    return true;
};


std::string getStringFromUser(const std::string &question, bool with_confirmation, std::string unallowed_chars) {

    std::string input;
    bool confirmed, valid = false;
    bool needs_validation = !unallowed_chars.empty();

    if (!needs_validation) {
        valid = true;
    }

    do {
        std::cout << question << std::endl << ">";

        std::getline(std::cin, input);

        if (needs_validation) {
            valid=validateString(input, unallowed_chars);
            if (!valid) {
                std::cout << "You cant have these characters in the input: \"" << unallowed_chars << "\"\n";
            }

        }

        if (with_confirmation && valid) {
            std::cout << "You've entered:" << input << std::endl;
            confirmed = getConfirmationFromUser("Is that right?");
        }

    } while ((with_confirmation && !confirmed) || !valid );

    return input;
};

std::string getStringFromUser(const std::string &question, bool with_confirmation) {
    return getStringFromUser(question, with_confirmation, "");
}

