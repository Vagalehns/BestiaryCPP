//
// Created by arman on 12/18/2025.
//

#include "Region.h"
#include "../../TUI_functions/Menu.h"


inline std::vector<Region> getDefaultBroadRegions() {
    std::vector<Region> defaultBroadRegions;

    for (int i = 0; i < FirstLevelRegionStrings.size(); i++) {
        defaultBroadRegions.emplace_back(static_cast<FirstLevelRegion>(i), FirstLevelRegionStrings[i]);
    }
    return defaultBroadRegions;
};

RegionDB::RegionDB(bool add_defaults) {
    if (add_defaults) {
        for (const auto &region: getDefaultBroadRegions()) {
            appendAutoID(region);
        }
    }
}

bool RegionDB::inputForm(Region &new_object, bool edit) {
    if (!edit || getConfirmationFromUser("Do you want to edit region name?")) {
        new_object.name = getStringFromUser("Write region name you want to add", true);
    }

    if (!edit || getConfirmationFromUser("Do you want to edit first level region?")) {
        new_object.first_level_region = static_cast<FirstLevelRegion>(getOptionFromUser(
            FirstLevelRegionStrings, "Pick first level region!"));
    }

    return true;
};

void RegionDB::filterWithOptions() {
    Menu filterMenu("Add filter/search", "View");

    filterMenu.addItem({
        "Search by name",
        ([this]() -> MenuReturn {
            std::string search_term = getStringFromUser("Please enter search term:", true);
            bool full_match = getConfirmationFromUser("Does term must be full match?");

            this->filterByField(&Region::name, makeGenericStringFilter(search_term, full_match));

            return {BACK, ""};
        })
    });

    filterMenu.addItem({
        "Filter by first level region",
        ([this]() -> MenuReturn {
            auto first_level_region = static_cast<FirstLevelRegion>(getOptionFromUser(
                FirstLevelRegionStrings, "Pick first level region!"));

            this->filterByField(&Region::first_level_region, [first_level_region](FirstLevelRegion flr) -> bool {
                return first_level_region == flr;
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


void RegionDB::preparedSort(char option) {
    switch (option) {
        case 'n':
            this->sort(&Region::name, genericStringSort);
            break;

        case 'f':
            this->sort(&Region::first_level_region, [](FirstLevelRegion A, FirstLevelRegion B) {
                return genericStringSort(FirstLevelRegionStrings[A], FirstLevelRegionStrings[B]);
            });
            break;

        default: ;
    };
};


void RegionDB::sortWithOptions() {
    Menu sortMenu("Pick sort option", "Don't sort");

    sortMenu.addItem({
        "Sort by name",
        ([this]() -> MenuReturn {
            preparedSort('n');
            return {BACK, ""};
        })
    });

    sortMenu.addItem({
        "Sort by first level region",
        ([this]() -> MenuReturn {
            preparedSort('f');
            return {BACK, ""};
        })
    });

    sortMenu.open();
};


std::vector<std::pair<std::string, char> > RegionDB::getViewColums(char view) {
    return {{"ID", 'i'}, {"Name", 's'}, {"First Level Region", 's'}};
};


std::vector<std::string> RegionDB::getAsStrings(Region &ref, char view) {
    std::vector<std::string> result;
    result.push_back(std::to_string(ref.ID));
    result.push_back(ref.name);
    result.emplace_back(FirstLevelRegionStrings[ref.first_level_region]);
    return result;
}

std::string RegionDB::convertToCSVLine(int index) {
    std::stringstream ss;
    ss << data[index].ID << CSV_SEP << data[index].name << CSV_SEP << data[index].first_level_region;
    return ss.str();
};

std::string RegionDB::getCSVHeader() {
    std::stringstream ss;
    ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "First level region";
    return ss.str();
};

Region RegionDB::getFromCSVline(std::string CSV_line) {
    std::string name, temp;
    std::stringstream ss(CSV_line);
    Region new_region;

    std::getline(ss, temp, CSV_SEP);
    new_region.ID = std::stoi(temp);

    std::getline(ss, temp, CSV_SEP);
    new_region.name = temp;

    std::getline(ss, temp, CSV_SEP);
    new_region.first_level_region = static_cast<FirstLevelRegion>(std::stoi(temp));

    return new_region;
};

bool RegionDB::isRecordOrphan(Region &ref) {
    return false;
}
