//
// Created by arman on 12/18/2025.
//

#include "Enclosure.h"

bool EnclosureDB::inputForm(Enclosure &new_object, bool edit) {
    if (!edit || getConfirmationFromUser("Do you want to edit name?")) {
        new_object.name = getStringFromUser("Write name of enclosure you want to add", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit section?")) {
        new_object.section = getStringFromUser("Write of section you want to add", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit type?")) {
        new_object.type = getOptionFromUser(EnclosuresTypes, "Pick enclosure type!");
    }
    return true;
};

void EnclosureDB::preparedSort(char option) {
    switch (option) {
        case 'n':
            this->sort(&Enclosure::name, genericStringSort);
            break;

        case 't':
            this->sort(&Enclosure::type, [](unsigned int A, unsigned int B) {
                return A > B;
            });
            break;

        case 's':
            this->sort(&Enclosure::section, genericStringSort);
            break;


        default: ;
    };
};


void EnclosureDB::sortWithOptions() {
    Menu sortMenu("Pick sort option", "Don't sort");

    sortMenu.addItem({
        "Sort by name",
        ([this]() -> MenuReturn {
            preparedSort('n');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by section",
        ([this]() -> MenuReturn {
            preparedSort('s');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by type",
        ([this]() -> MenuReturn {
            preparedSort('t');
            return {BACK, ""};
        })
    });


    sortMenu.open();
};


void EnclosureDB::filterWithOptions() {
    Menu filterMenu("Add filter/search", "View");

    filterMenu.addItem({
        "Search by name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Enclosure::name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });

    filterMenu.addItem({
        "Filter by type",
        ([this]() -> MenuReturn {
            auto type = getOptionFromUser(EnclosuresTypes, "Pick enclosure type!");

            this->filterByField(&Enclosure::type, [type](int cur_type) -> bool {
                return type == cur_type;
            });

            return {STAY, ""};
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

std::vector<std::pair<std::string, char> > EnclosureDB::getViewColums(char view) {
    return {{"ID", 'i'}, {"Name", 's'}, {"Section", 's'}, {"Type", 's'}};
};

std::vector<std::string> EnclosureDB::getAsStrings(Enclosure &ref, char view) {
    std::vector<std::string> result;
    result.push_back(std::to_string(ref.ID));
    result.push_back(ref.name);
    result.push_back(ref.section);
    result.emplace_back(EnclosuresTypes[ref.type]);
    return result;
}


std::string EnclosureDB::convertToCSVLine(int index) {
    std::stringstream ss;
    auto di = data[index];
    ss << di.ID << CSV_SEP << di.name << CSV_SEP << di.section << CSV_SEP << di.type;
    return ss.str();
};

std::string EnclosureDB::getCSVHeader() {
    std::stringstream ss;
    ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Section" << CSV_SEP << "Type";
    return ss.str();
}

Enclosure EnclosureDB::getFromCSVline(std::string CSV_line) {
    std::string name, temp;
    std::stringstream ss(CSV_line);
    Enclosure new_enclosure;

    std::getline(ss, temp, CSV_SEP);
    new_enclosure.ID = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_enclosure.name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_enclosure.section = temp;

    std::getline(ss, temp, CSV_SEP);
    new_enclosure.type = std::stoi(temp);

    return new_enclosure;
};

bool EnclosureDB::isRecordOrphan(Enclosure &ref) {
    return false;
}
