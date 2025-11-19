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

std::string generateTable(std::vector<std::string> data, int columns, bool borders, bool number_items) {

    unsigned short longest_item_len=0;

    std::string table;

    for (const auto& elem : data) {

        if (elem.length()>longest_item_len) {
            longest_item_len = elem.length();
        }
    }

    //extra padding
    longest_item_len+=2;

    int item_amount=data.size();

    if (number_items) {
        longest_item_len += item_amount/10+2;
    }


    int table_hor_size= (longest_item_len+1) * columns + 1;

    std::string hor_div=std::string(table_hor_size, '-');




    int row_amount = ( item_amount + columns - 1) / columns;

    if (borders) table.append(hor_div+"\n");

    for (int i = 0; i < row_amount; ++i) {

        for (int j = 0; j < columns; ++j) {

            if (borders)table.append("|");

            int table_index = i*columns+j ;

            if (table_index<item_amount) {
                if (number_items) {
                    table.append(padString( std::to_string(table_index+1) + "." + data[table_index], longest_item_len, ' ', true));
                }else {
                    table.append(padString(data[table_index], longest_item_len, ' ', true));
                }

            }else {
                table.append(std::string(longest_item_len, ' '));
            }

        }

        if (borders) table.append("|");
        table.append("\n");
        if (borders)table.append(hor_div);
        table.append("\n");
    }

    return table;
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
        std::cout << question << "\n>";

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

void Menu::open(const bool clear){

    if (clear) {
        clearConsole();
    }

    std::cout << BOLDSTY BGBRIGHTGREENSTY BLACKSTY << "~{ " << menu_title << " }~" << ENDSTY << "\n";

    // Items are displayed in the reverse order
    for (int i = item_count - 1; i >= 0; i--) {
        std::cout << (item_count - i) << ". " << BRIGHTGREENSTY << menu_items[i].title << ENDSTY << "\n";
    }


    while (!should_close) {
        int selection = getIntFromUser(1, item_count, ">", true, true);
        bool fine = menu_items[this->translateFromUserToIndex(selection)].screen_to_call();

        if (!fine) {
            std::cout << "\nCouldn't fulfill the action\n";
        };
    }

    if (clear) {
        clearConsole();
    }

}
