//
// Created by armands on 14.11.25.
//

#include "TUI_functions.h"

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "styling_functions.h"
#include <string>

#include "info.h"


void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}


//Sonnet 2.5
size_t getProperStringLength(const std::string &str) {
    size_t length = 0;
    size_t i = 0;

    while (i < str.size()) {
        uint8_t byte = static_cast<uint8_t>(str[i]);

        // Determine how many bytes this character uses
        if ((byte & 0x80) == 0) {
            // 1-byte character (0xxxxxxx)
            i += 1;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte character (110xxxxx)
            i += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte character (1110xxxx)
            i += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte character (11110xxx)
            i += 4;
        } else {
            // Invalid UTF-8, skip this byte
            i += 1;
        }

        length++;
    }

    return length;
};

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

    unsigned short padding_amount=new_size - getProperStringLength(string_to_pad);

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




long long int getIntFromUser(long long int  min, long long int max, const std::string &question, bool has_upper_bound, bool has_lower_bound) {

    long long int  input;
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
        std::cout << "[" GREENSTY "y" ENDSTY "/" REDSTY "n" ENDSTY "]";

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

    std::cout << BGBRIGHTBLUESTY "~" << question << "~" ENDSTY;
    do {

        std::cout << std::endl << ">";

        std::getline(std::cin, input);

        if (needs_validation) {
            valid=validateString(input, unallowed_chars);
            if (!valid) {
                std::cout << "You cant have these characters in the input: \"" << unallowed_chars << "\"\n";
            }

        }

        if (with_confirmation && valid) {
            std::cout << BOLDSTY BLUESTY << "\nYou've entered: " << ENDSTY << input << std::endl;
            confirmed = getConfirmationFromUser("Is that right?");
        }

    } while ((with_confirmation && !confirmed) || !valid );

    return input;
};

std::string formatTime(std::time_t t, const std::string &format) {
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, format.c_str());
    return oss.str();
}

std::string formatTime(std::time_t t) {
    return formatTime(t, DEFAULT_TIME_FORMAT);
}

std::time_t getTimeFromUser(const std::string &question) {
    return getTimeFromUser(question, DEFAULT_TIME_FORMAT);
};

std::time_t getTimeFromUser(const std::string &question, std::string time_format) {

    std::string input;
    std::tm timeStruct = {};
    std::time_t result;

    bool confirmed = false;
    do {
        std::cout << question << " (format: " << time_format << "): ";
        std::getline(std::cin, input);

        timeStruct = {};
        std::istringstream ss(input);

        ss >> std::get_time(&timeStruct, (time_format.c_str()));

        if (ss.fail() || !ss.eof()) {
            std::cout << "Invalid time format. Please try again.\n";
            continue;
        }

        if (timeStruct.tm_year == 0) timeStruct.tm_year = 100;
        if (timeStruct.tm_mon == 0 && time_format.find("%m") == std::string::npos) {
            timeStruct.tm_mon = 0;
        }
        if (timeStruct.tm_mday == 0) timeStruct.tm_mday = 1;

        result = std::mktime(&timeStruct);

        if (result == -1) {
            std::cout << "Invalid date/time. Please try again.\n";
            continue;
        }

        std::cout << "You've entered: " << formatTime(result, time_format) << std::endl;
        confirmed = getConfirmationFromUser("Is that right?");

    } while (!confirmed);

    return result;
}

std::string getStringFromUser(const std::string &question, bool with_confirmation) {
    return getStringFromUser(question, with_confirmation, "");
}
std::string getStringFromUserWithPattern(const std::string &question, std::string error_message, std::function<bool(std::string)> checkFunc) {

    std::string user_string;
    bool correct, confirmed=false;

    do {
        user_string = getStringFromUser(question, false);
        correct = checkFunc(user_string);
        if (!correct) {
            std::cout<<error_message<<"\n";
        }else {
            std::cout << BOLDSTY BLUESTY << "\nYou've entered: " << ENDSTY << user_string << std::endl;
            confirmed = getConfirmationFromUser("Is that right?");
        }





    } while (!(correct&&confirmed));

    return user_string;

};

