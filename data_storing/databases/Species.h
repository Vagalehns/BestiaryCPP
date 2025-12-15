//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_SPECIES_H
#define BESTIARYCPP_SPECIES_H


#include "../genericDatabase.h"
#include "../../TUI_functions/TUI_functions.h"
#include "Region.h"

struct Species : DefaultStruct {

    std::string name;
    std::string latin_name;
    unsigned int icon;
    ExternalKey<Region> RegionID;

    void display() {
        std::cout << "\n Name: " << name ;
        std::cout << "\n Latin Name: " << latin_name ;
        std::cout << "\n Icon: " << SpeciesEmojiIcons[icon] ;
        std::cout << "\n Region ID: " << RegionID.ID << std::endl;
    };

    Species(){};

    Species(std::string name, std::string latin_name, unsigned int icon, ExternalKey<Region> regionID) {
        this->name = name;
        this->latin_name = latin_name;
        this->icon = icon;
        this->RegionID = regionID;
    }
};


class SpeciesDB : public DB<Species, MAX_SPECIES> {
    std::function<Region*(KeyID)> region_resolver;
    std::function<KeyID(bool &)> region_picker;
public:

    SpeciesDB (std::function<Region*(KeyID)> region_resolver, std::function<KeyID(bool &)> region_picker) {
        this->region_resolver = region_resolver;
        this->region_picker = region_picker;
    }

    bool inputForm(Species &new_object, bool edit) override {

        if (!edit || getConfirmationFromUser("Do you want to edit species name?")) {
            new_object.name=getStringFromUser("Write species name", true);
        }
        clearConsole();

        if (!edit || getConfirmationFromUser("Do you want to edit species latin name?")) {
            new_object.latin_name=getStringFromUser("Write species latin name", true);
        }
        clearConsole();

        if (!edit || getConfirmationFromUser("Do you want to edit species icon?")) {
            new_object.icon=getOptionFromUser(SpeciesEmojiIcons, "Pick species icon");
        }

        bool success;
        if (!edit || getConfirmationFromUser("Do you want to edit region?")) {
            new_object.RegionID=ExternalKey(region_picker(success), region_resolver);
            if (!success) return false;
        }

        return true;
    };


    virtual void filterOptions() {

    }


    void sortOptions() override {
        Menu sortMenu("Pick sort option", "Don't sort");

        sortMenu.addItem({
            "Sort by name",
            ([this]() -> MenuReturn {
                this->sort(&Species::name, genericStringSort);
                return {BACK, ""};
            })
        });

        sortMenu.addItem({
            "Sort by latin name",
            ([this]() -> MenuReturn {
                this->sort(&Species::latin_name, genericStringSort);
                return {BACK, ""};
            })
        });


        sortMenu.addItem({
            "Sort by region",
            ([this]() -> MenuReturn {

                this->sort(&Species::RegionID, [](ExternalKey<Region> A, ExternalKey<Region> B) -> int {

                    auto a = A.get();
                    auto b = B.get();

                    if (a == nullptr)  return 1;

                    if (b == nullptr) return -1;

                    return b->name.compare(a->name);
                });

                return {BACK, ""};
            })
        });

        sortMenu.open();
    }


    std::vector< std::pair< std::string, char > > getViewColums(char view)  override {
        return {{"ID", 'i'}, {"Name", 's'}, {"Latin name", 's'}, {"Icon", 's'}, {"Region", 's'}};
    };

    std::vector<std::string> getAsStrings(Species &ref, char view) override {
        std::vector<std::string> result;


        result.push_back(std::to_string(ref.ID));
        result.push_back(ref.name);
        result.push_back(ref.latin_name);
        result.push_back(SpeciesEmojiIcons[ref.icon]);

        Region *region=ref.RegionID.get();

        if (region==nullptr) {
            result.push_back("Unvalid key");
        }else {
            result.push_back(region->name);
        };

        return result;
    }

        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            auto di=data[index];
            ss<<di.ID<<CSV_SEP<<di.name<<CSV_SEP<<di.latin_name<<CSV_SEP<<di.icon<<CSV_SEP<<di.RegionID.ID;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Latin name" << CSV_SEP << "Icon" << CSV_SEP << "Region ID";
            return ss.str();
        };

        Species getFromCSVline(std::string CSV_line) override {

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
            new_species.RegionID= ExternalKey(std::stoi(temp), region_resolver);

            return new_species;
        };
};


#endif //BESTIARYCPP_SPECIES_H