//
// Created by arman on 12/18/2025.
//

#include "Species.h"

SpeciesDB::SpeciesDB(const std::function<Region*(KeyID)> &region_resolver,
                     const std::function<KeyID(bool &, std::string)> &region_picker) {
    this->region_resolver = region_resolver;
    this->region_picker = region_picker;
}

bool SpeciesDB::inputForm(Species &new_object, bool edit) {
    if (!edit || getConfirmationFromUser("Do you want to edit species name?")) {
        new_object.name = getStringFromUser("Write species name", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit species latin name?")) {
        new_object.latin_name = getStringFromUser("Write species latin name", true);
    }
    clearConsole();

    if (!edit || getConfirmationFromUser("Do you want to edit species icon?")) {
        new_object.icon = getOptionFromUser(SpeciesEmojiIcons, "Pick species icon");
    }

    if (!edit || getConfirmationFromUser("Do you want to edit region?")) {
        bool success;

        new_object.region = ExternalKey(region_picker(success, "Pick the region species come from"), region_resolver);
        if (!success) return false;
    }

    return true;
};


void SpeciesDB::filterWithOptions() {
    Menu filterMenu("Add filter/search", "View");

    filterMenu.addItem({
        "Search by name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Species::name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });

    filterMenu.addItem({
        "Search by latin name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Species::latin_name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });


    filterMenu.addItem({
        "Search by region",
        ([this]() -> MenuReturn {
            bool success;
            auto r = region_picker(success, "Select the region");

            if (!success) {
                return {STAY_SHOW_ERROR, "Picking the region failed"};
            }

            this->filterByField(&Species::region, [r](const ExternalKey<Region> &region) -> bool {
                return region.ID == r;
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

void SpeciesDB::preparedSort(char option) {
    switch (option) {
        case 'n':
            this->sort(&Species::name, genericStringSort);
            break;

        case 'l':
            this->sort(&Species::latin_name, genericStringSort);
            break;

        case 'r':
            this->sort(&Species::region, [](ExternalKey<Region> A, ExternalKey<Region> B) -> int {
                const auto a = A.get();
                const auto b = B.get();

                if (a == nullptr) return 1;

                if (b == nullptr) return -1;

                return b->name.compare(a->name);
            });

        default: ;
    };
};

void SpeciesDB::sortWithOptions() {
    Menu sortMenu("Pick sort option", "Don't sort");

    sortMenu.addItem({
        "Sort by name",
        ([this]() -> MenuReturn {
            preparedSort('n');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by latin name",
        ([this]() -> MenuReturn {
            preparedSort('l');
            return {BACK, ""};
        })
    });


    sortMenu.addItem({
        "Sort by region",
        ([this]() -> MenuReturn {
            preparedSort('r');
            return {BACK, ""};
        })
    });

    sortMenu.open();
}


std::vector<std::pair<std::string, char> > SpeciesDB::getViewColums(char view) {
    return {{"ID", 'i'}, {"Name", 's'}, {"Latin name", 's'}, {"Icon", 's'}, {"Region", 's'}};
};

std::vector<std::string> SpeciesDB::getAsStrings(Species &ref, char view) {
    std::vector<std::string> result;


    result.push_back(std::to_string(ref.ID));
    result.push_back(ref.name);
    result.push_back(ref.latin_name);
    result.emplace_back(SpeciesEmojiIcons[ref.icon]);

    Region *region = ref.region.get();

    if (region == nullptr) {
        result.emplace_back("Invalid key");
    } else {
        result.push_back(region->name);
    };

    return result;
}

std::string SpeciesDB::convertToCSVLine(int index) {
    std::stringstream ss;
    auto di = data[index];
    ss << di.ID << CSV_SEP << di.name << CSV_SEP << di.latin_name << CSV_SEP << di.icon << CSV_SEP << di.region.ID;
    return ss.str();
};

std::string SpeciesDB::getCSVHeader() {
    std::stringstream ss;
    ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Latin name" << CSV_SEP << "Icon" << CSV_SEP << "Region ID";
    return ss.str();
};

Species SpeciesDB::getFromCSVline(std::string CSV_line) {
    std::string name, temp;
    std::stringstream ss(CSV_line);
    Species new_species;

    std::getline(ss, temp, CSV_SEP);
    new_species.ID = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_species.name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_species.latin_name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_species.icon = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_species.region = ExternalKey(std::stoi(temp), region_resolver);

    return new_species;
};

bool SpeciesDB::isRecordOrphan(Species &ref) {
    return ref.region.get() == nullptr;
}
