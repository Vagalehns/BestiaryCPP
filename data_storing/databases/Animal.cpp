//
// Created by arman on 12/18/2025.
//

#include "Animal.h"

#include "../genericDB.h"
#include "../../TUI_functions/TUI_functions.h"
#include "../../TUI_functions/Menu.h"

bool AnimalDB::inputForm(Animal &new_object, bool edit) {
    if (!edit || getConfirmationFromUser("Do you want to edit name?")) {
        new_object.name = getStringFromUser("Write name you want to add", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit DOB?")) {
        new_object.date_of_birth = getTimeFromUser("Write DOB");
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit Last vet check date?")) {
        new_object.last_veterinary_check = getTimeFromUser("Write Last vet check");
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit Last date of weighing?")) {
        new_object.date_of_weighting = getTimeFromUser("Write Last date of weighing");
    }

    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit sex?")) {
        new_object.sex = getConfirmationFromUser("Is the animal male?");
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit weight?")) {
        WeightMeasurement wm = static_cast<WeightMeasurement>(getOptionFromUser(
            WeightMeasurementFullName, "Pick measurement type!"));
        new_object.weight = Weight(getIntFromUser(0, -1, "Input weight:", false, true), wm);
    }

    bool success;

    if (!edit || getConfirmationFromUser("Do you want to edit species?")) {
        new_object.species = ExternalKey(species_picker(success, "Pick the species of the animal"), species_resolver);
        if (!success) return false;
    }

    if (!edit || getConfirmationFromUser("Do you want to edit keeper?")) {
        new_object.keeper = ExternalKey(keeper_picker(success, "Pick the keeper of the animal"), keeper_resolver);
        if (!success) return false;
    }

    if (!edit || getConfirmationFromUser("Do you want to edit enclosure?")) {
        new_object.enclosure = ExternalKey(enclosure_picker(success, "Pick the enclosure"), enclosure_resolver);
        if (!success) return false;
    }

    return true;
};


void AnimalDB::filterWithOptions() {
    Menu filterMenu("Add filter/search", "View");

    filterMenu.addItem({
        "Search by name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Animal::name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });

    filterMenu.addItem({
        "Search by species",
        ([this]() -> MenuReturn {
            bool success;
            auto s = species_picker(success, "Select the species to search by");

            if (!success) {
                return {STAY_SHOW_ERROR, "Picking the species failed"};
            }

            this->filterByField(&Animal::species, [s](const ExternalKey<Species> &species) -> bool {
                return species.ID == s;
            });

            return {STAY, ""};
        })
    });

    filterMenu.addItem({
        "Search by keeper",
        ([this]() -> MenuReturn {
            bool success;
            auto k = keeper_picker(success, "Select the keeper of the animal");

            if (!success) {
                return {STAY_SHOW_ERROR, "Picking the keeper failed"};
            }

            this->filterByField(&Animal::keeper, [k](const ExternalKey<Keeper> &keeper) -> bool {
                return keeper.ID == k;
            });

            return {STAY, ""};
        })
    });

    filterMenu.addItem({
        "Search by enclosure",
        ([this]() -> MenuReturn {
            bool success;
            auto k = keeper_picker(success, "Select the enclosure");

            if (!success) {
                return {STAY_SHOW_ERROR, "Picking the enclosure failed"};
            }

            this->filterByField(&Animal::enclosure, [k](const ExternalKey<Enclosure> &enclosure) -> bool {
                return enclosure.ID == k;
            });

            return {STAY, ""};
        })
    });

    filterMenu.addItem({
        "Filter by weight",
        ([this]() -> MenuReturn {
            WeightMeasurement wm = static_cast<WeightMeasurement>(getOptionFromUser(
                WeightMeasurementFullName, "Pick measurement type!"));
            auto lower_bound = getIntFromUser(0, -1, "Pick lower weight bound: ", false, true);
            auto upper_bound = getIntFromUser(lower_bound, -1, "Pick upper weight bound: ", false, true);

            Weight min(lower_bound, wm), max(upper_bound, wm);

            this->filterByField(&Animal::weight, [min, max](Weight weight) -> bool {
                return weight.weight_in_mg >= min.weight_in_mg && weight.weight_in_mg <= max.weight_in_mg;
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


void AnimalDB::preparedSort(char option) {
    switch (option) {
        case 'n':
            this->sort(&Animal::name, genericStringSort);
            break;

        case 'w':
            this->sort(&Animal::weight, [](Weight a, Weight b) {
                if (a.weight_in_mg < b.weight_in_mg) return 1;
                if (a.weight_in_mg > b.weight_in_mg) return -1;
                return 0;
            });
            break;

        case 's':
            this->sort(&Animal::sex, [](bool a, bool b) {
                if (a == b) return 0;
                if (a) return 1;
                return -1;
            });
            break;

        case 'd':
            this->sort(&Animal::date_of_birth, genericTimeSort);
            break;

        case 'v':
            this->sort(&Animal::last_veterinary_check, genericTimeSort);
            break;

        case 'g':
            this->sort(&Animal::date_of_weighting, genericTimeSort);
            break;

        case 'p':
            this->sort(&Animal::species, [](ExternalKey<Species> A, ExternalKey<Species> B) -> int {
                auto a = A.get();
                auto b = B.get();

                if (a == nullptr) return 1;

                if (b == nullptr) return -1;

                return b->name.compare(a->name);
            });
            break;

        case 'k':
            this->sort(&Animal::keeper, [](ExternalKey<Keeper> A, ExternalKey<Keeper> B) -> int {
                auto a = A.get();
                auto b = B.get();

                if (a == nullptr) return 1;

                if (b == nullptr) return -1;

                return b->name.compare(a->name);
            });
            break;

        case 'e':
            this->sort(&Animal::enclosure, [](ExternalKey<Enclosure> A, ExternalKey<Enclosure> B) -> int {
                auto a = A.get();
                auto b = B.get();

                if (a == nullptr) return 1;

                if (b == nullptr) return -1;

                return b->name.compare(a->name);
            });
        default: ;
    };
};

void AnimalDB::sortWithOptions() {
    Menu sortMenu("Pick sort option", "Don't sort");

    sortMenu.addItem({
        "Sort by name",
        ([this]() -> MenuReturn {
            preparedSort('n');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by weight",
        ([this]() -> MenuReturn {
            preparedSort('w');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by sex",
        ([this]() -> MenuReturn {
            preparedSort('s');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by DOB",
        ([this]() -> MenuReturn {
            preparedSort('d');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by last veterinary check",
        ([this]() -> MenuReturn {
            preparedSort('v');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by date of weighing",
        ([this]() -> MenuReturn {
            preparedSort('g');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by species",
        ([this]() -> MenuReturn {
            preparedSort('p');

            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by keeper",
        ([this]() -> MenuReturn {
            preparedSort('k');

            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by enclosure",
        ([this]() -> MenuReturn {
            preparedSort('e');
            return {BACK, ""};
        })
    });

    sortMenu.open();
}


std::vector<std::pair<std::string, char> > AnimalDB::getViewColums(char view) {
    return {
        {"ID", 'i'}, {"Name", 's'}, {"Sex", 's'}, {"Species", 's'}, {"Keeper", 's'}, {"Enclosure", 's'}, {"DOB", 's'},
        {"Weight", 'i'}, {"Last veterinary check", 's'}
    };
};

std::vector<std::string> AnimalDB::getAsStrings(Animal &ref, char view) {
    std::vector<std::string> result;

    result.push_back(std::to_string(ref.ID));
    result.push_back(ref.name);
    result.emplace_back(ref.sex ? "Male" : "Female");


    Species *s = ref.species.get();
    if (s != nullptr) {
        result.push_back(s->name);
    } else {
        result.emplace_back("Invalid key");
    }

    Keeper *k = ref.keeper.get();
    if (k != nullptr) {
        result.push_back(k->name + " " + k->surname);
    } else {
        result.emplace_back("Invalid key");
    }

    Enclosure *e = ref.enclosure.get();
    if (e != nullptr) {
        result.push_back(e->name);
    } else {
        result.emplace_back("Invalid key");
    }

    result.push_back(formatTime(ref.date_of_birth));
    result.push_back(ref.weight.display());
    result.push_back(formatTime(ref.last_veterinary_check));

    return result;
}

std::string AnimalDB::convertToCSVLine(int index) {
    std::stringstream ss;
    auto di = data[index];


    ss << di.ID << CSV_SEP;
    ss << di.name << CSV_SEP;
    ss << di.weight.weight_in_mg << CSV_SEP;
    ss << di.weight.preferred_measurement << CSV_SEP;
    ss << di.sex << CSV_SEP;
    ss << di.date_of_birth << CSV_SEP;
    ss << di.last_veterinary_check << CSV_SEP;
    ss << di.date_of_weighting << CSV_SEP;
    ss << di.species.ID << CSV_SEP;
    ss << di.keeper.ID << CSV_SEP;
    ss << di.enclosure.ID;

    return ss.str();
};

std::string AnimalDB::getCSVHeader() {
    std::stringstream ss;
    ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Weight" << CSV_SEP << "Weight.preferred_measurements" << CSV_SEP <<
            "Sex" << CSV_SEP
            << "Date of Birth" << CSV_SEP << "Last Vet Check" << CSV_SEP << "Date of Weighting" << CSV_SEP
            << "SpeciesID" << CSV_SEP << "KeeperID" << CSV_SEP << "EnclosureID";
    return ss.str();
}

Animal AnimalDB::getFromCSVline(std::string CSV_line) {
    std::string name, temp;
    std::stringstream ss(CSV_line);
    Animal new_animal;

    std::getline(ss, temp, CSV_SEP);
    new_animal.ID = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_animal.name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_animal.weight.weight_in_mg = std::stoul(temp);

    std::getline(ss, temp, CSV_SEP);
    new_animal.weight.preferred_measurement = static_cast<WeightMeasurement>(std::stoi(temp));

    std::getline(ss, temp, CSV_SEP);
    new_animal.sex = static_cast<bool>(std::stoi(temp));

    std::getline(ss, temp, CSV_SEP);
    new_animal.date_of_birth = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_animal.last_veterinary_check = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_animal.date_of_weighting = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_animal.species = ExternalKey(std::stoi(temp), species_resolver);

    std::getline(ss, temp, CSV_SEP);
    new_animal.keeper = ExternalKey(std::stoi(temp), keeper_resolver);

    std::getline(ss, temp, CSV_SEP);
    new_animal.enclosure = ExternalKey(std::stoi(temp), enclosure_resolver);

    return new_animal;
};

bool AnimalDB::isRecordOrphan(Animal &ref) {
    return ref.keeper.get() == nullptr ||
           ref.enclosure.get() == nullptr ||
           ref.species.get() == nullptr;
}
