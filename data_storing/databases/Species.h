//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_SPECIES_H
#define BESTIARYCPP_SPECIES_H


#include "../genericDB.h"
#include "../../TUI_functions/TUI_functions.h"
#include "../../TUI_functions/Menu.h"
#include "Region.h"

struct Species : DefaultStruct {
    std::string name;
    std::string latin_name;
    unsigned int icon;
    ExternalKey<Region> region;

    void display() const {
        std::cout << "\n Name: " << name;
        std::cout << "\n Latin Name: " << latin_name;
        std::cout << "\n Icon: " << SpeciesEmojiIcons[icon];
        std::cout << "\n Region ID: " << region.ID << std::endl;
    };

    Species() : icon(0) {
    } ;

    Species(const std::string &name, const std::string &latin_name, unsigned int icon,
            const ExternalKey<Region> &regionID) {
        this->name = name;
        this->latin_name = latin_name;
        this->icon = icon;
        this->region = regionID;
    }
};


class SpeciesDB : public DB<Species, MAX_SPECIES> {
    std::function<Region*(KeyID)> region_resolver;
    std::function<KeyID(bool &, std::string)> region_picker;

public:
    virtual ~SpeciesDB() = default;

    SpeciesDB(const std::function<Region*(KeyID)> &region_resolver,
              const std::function<KeyID(bool &, std::string)> &region_picker);

    bool inputForm(Species &new_object, bool edit) override;

    void filterWithOptions() override;

    void preparedSort(char option) override;

    void sortWithOptions() override;


    std::vector<std::pair<std::string, char> > getViewColums(char view) override;

    std::vector<std::string> getAsStrings(Species &ref, char view) override;

    std::string convertToCSVLine(int index) override;

    std::string getCSVHeader() override;

    Species getFromCSVline(std::string CSV_line) override;

    bool isRecordOrphan(Species &ref) override;
};


#endif //BESTIARYCPP_SPECIES_H
