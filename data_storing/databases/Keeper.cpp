//
// Created by arman on 12/18/2025.
//

#include "Keeper.h"
#include "../../TUI_functions/Menu.h"

bool KeeperDB::inputForm(Keeper &new_object, bool edit) {
    if (!edit || getConfirmationFromUser("Do you want to edit name?")) {
        new_object.name = getStringFromUser("Write name you want to add", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit surname?")) {
        new_object.surname = getStringFromUser("Write surname you want to add", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit phone number?")) {
        new_object.phone_number = getStringFromUserWithPattern("Input phone number", PhoneNumber::Error_Message,
                                                               PhoneNumber::checkPhoneNumber);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit address?")) {
        std::cout << "Enter address:\n";
        new_object.address.city = getStringFromUser("Write city", true);
        new_object.address.street = getStringFromUser("Write street and number", true);
        new_object.address.zip_code = getStringFromUser("Write zip code", true);
    }
    return true;
};

void KeeperDB::filterWithOptions() {
    Menu filterMenu("Add filter/search", "View");

    filterMenu.addItem({
        "Search by name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Keeper::name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });

    filterMenu.addItem({
        "Search by surname",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Keeper::surname, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });


    filterMenu.addItem({
        "Clear filters",
        ([this]() -> MenuReturn {
            this->resetFilter();

            return {BACK, ""};
        })
    });

    filterMenu.open();
}

void KeeperDB::preparedSort(char option) {
    switch (option) {
        case 'n':
            this->sort(&Keeper::name, genericStringSort);
            break;

        case 's':
            this->sort(&Keeper::surname, genericStringSort);
            break;

        case 'a':
            this->sort(&Keeper::address, [](const Address &A, const Address &B) {
                return genericStringSort(A.toString(), B.toString());
            });
            break;


        default: ;
    };
};

void KeeperDB::sortWithOptions() {
    Menu sortMenu("Pick sort option", "Don't sort");

    sortMenu.addItem({
        "Sort by name",
        ([this]() -> MenuReturn {
            preparedSort('n');
            return {BACK, ""};
        })
    });


    sortMenu.addItem({
        "Sort by surname",
        ([this]() -> MenuReturn {
            preparedSort('s');
            return {BACK, ""};
        })
    });


    sortMenu.addItem({
        "Sort by address",
        ([this]() -> MenuReturn {
            preparedSort('a');
            return {BACK, ""};
        })
    });


    sortMenu.open();
};

std::vector<std::pair<std::string, char> > KeeperDB::getViewColums(char view) {
    return {{"ID", 'i'}, {"Name", 's'}, {"Surname", 's'}, {"Phone number", 's'}, {"Address", 's'}};
};


std::vector<std::string> KeeperDB::getAsStrings(Keeper &ref, char view) {
    std::vector<std::string> result;
    result.push_back(std::to_string(ref.ID));
    result.push_back(ref.name);
    result.push_back(ref.surname);
    result.push_back(ref.phone_number.number);
    result.push_back(ref.address.toString());
    return result;
}


std::string KeeperDB::convertToCSVLine(int index) {
    std::stringstream ss;
    auto di = data[index];
    ss << di.ID << CSV_SEP << di.name << CSV_SEP << di.surname << CSV_SEP
            << di.address.city << CSV_SEP << di.address.street << CSV_SEP << di.address.zip_code
            << CSV_SEP << di.phone_number.number;
    return ss.str();
};

std::string KeeperDB::getCSVHeader() {
    std::stringstream ss;
    ss << "ID" << CSV_SEP
            << "Name" << CSV_SEP
            << "Surname" << CSV_SEP
            << "Address.city" << CSV_SEP << "Address.street" << CSV_SEP << "Address.zipcode" << CSV_SEP
            << "Phone number";
    return ss.str();
};

Keeper KeeperDB::getFromCSVline(std::string CSV_line) {
    std::string name, temp;
    std::stringstream ss(CSV_line);
    Keeper new_keeper;

    std::getline(ss, temp, CSV_SEP);
    new_keeper.ID = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_keeper.name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_keeper.surname = temp;

    std::getline(ss, temp, CSV_SEP);
    new_keeper.address.city = temp;
    std::getline(ss, temp, CSV_SEP);
    new_keeper.address.street = temp;
    std::getline(ss, temp, CSV_SEP);
    new_keeper.address.zip_code = temp;

    std::getline(ss, temp, CSV_SEP);
    new_keeper.phone_number = temp;

    return new_keeper;
};

bool KeeperDB::isRecordOrphan(Keeper &ref) {
    return false;
}
